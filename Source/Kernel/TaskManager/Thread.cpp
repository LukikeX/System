#include "Thread.h"
#include "Task.h"
#include "Core/IO.h"
#include "DeviceManager/Time.h"

void Thread::run(Thread* t, void* data, ThreadEntry entryPoint) {
    t->process->getPageDir()->switchTo();
    
    if (t->isKernel) {
        IO::cli();
        ulong ret = entryPoint(data);
        asm volatile ("mov %0, %%rax\n int $66" :: "r"(ret));
    } else {
        ulong* stack = (ulong *)((ulong)t->userStack.address + t->userStack.size);
        stack--;
        *stack = (ulong)data;
        stack--;
        *stack = 0;
        
        ulong rsp = (ulong)stack;
        ulong rip = (ulong)entryPoint;
        
        asm volatile ("mov $0x23, %%ax \n"
                      "mov %%ax, %%ds \n"
                      "mov %%ax, %%es \n"
                      "mov %%ax, %%fs \n"
                      "mov %%ax, %%gs \n"
                      
                      "mov %0, %%rbx \n"
                      "mov %1, %%rcx \n"
                      "push $0x23 \n"
                      "push %%rbx \n"
                      "pushf \n"
                      "pop %%rax \n"
                      "or $0x200, %%rax \n"
                      "push %%rax \n"
                      "push $0x1B \n"
                      "iret"
                      : : "r"(rsp), "r"(rip)
        );
    }
}

Thread::Thread() {
    xchSpace = 0;
}

Thread::Thread(ThreadEntry entryPoint, void* data, bool isKernel) {
    if (isKernel)
        setup(Task::getKernelProcess(), entryPoint, data, true);
    else
        setup(Task::currentProcess(), entryPoint, data, false);
}

Thread::Thread(Process* process, ThreadEntry entryPoint, void* data) {
    setup(process, entryPoint, data, false);
}

Thread::~Thread() {
    Task::unregisterThread(this);
    delete (ulong *)kernelStack.address;
    process->getPageDir()->switchTo();
    if (userStack.address && !isKernel)
        process->heap().free(userStack.address);
    
    if (xchSpace)
        process->heap().free(xchSpace);
}

void Thread::setup(Process* process, ThreadEntry entryPoint, void* data, bool isKernel) {
    xchSpace = 0;
    this->isKernel = isKernel;
    this->process = process;
    kernelStack.address = new char[STACKSIZE];
    kernelStack.size = STACKSIZE;
    
    if (isKernel) {
        userStack.address = 0;
        userStack.size = 0;
    } else {
        userStack.address = process->heap().alloc(STACKSIZE);
        userStack.size = STACKSIZE;
    }
    
    ulong* stack = (ulong *)((ulong)kernelStack.address + kernelStack.size);
    stack--;
    *stack = (ulong)entryPoint;
    stack--;
    *stack = (ulong)data;
    stack--;
    *stack = (ulong)this;
    stack--;
    *stack = 0;
    
    rsp = (ulong)stack;
    rbp = rsp + 8;
    rip = (ulong)run;
    
    state = RUNNING;
    process->registerThread(this);
    Task::registerThread(this);
}

void Thread::finish(uint errorCode) {
    if (errorCode == E_PAGEFAULT && isKernel)
        panic("Page fault in kernel thread!");
    
    if (E_UNHLD_EXCP && isKernel)
        panic("Unhandled exception in kernel thread!");
    
    process->threadFinishes(this, errorCode);
}

void Thread::handleException(IDT::regs* r) {
    const char* exceptions[] = {
        "Division by zero", "Debug exception", "Non maskable interrupt",
        "Breakpoint exception", "'Into detected overflow'", "Out of bounds exception",
        "Invalid opcode exception", "No coprocessor exception", "Double fault",
        "Coprocessor segment overrun", "Bad TSS", "Segment not present",
        "Stack fault", "General protection fault", "Page fault",
        "Unknown", "Coprocessor fault", "Alignement check exception",
        "Machine check exception", "", "", "", "", "", "", "", "", "", "", "", "", ""};
    
    VirtualTerminal& vt = *(process->getOutVT());
    vt << "\nUnhandled exception " << r->intNo << " at " << r->cs << ":"
       << r->rip << "\n:: " << exceptions[r->intNo];
    
    if (r->intNo == 3) {
        vt << "\n\nBreakpoint data:\n";
        dumpRegisters(r, &vt);
        vt << "Press any key to continue execution of program...";
        process->getInVT()->getkeyPress(false);
        vt << "\n";
        return;
    }
    
    if (isKernel)
        panic("Exception in kernel thread");
    
    if (r->intNo == 14) {
        ulong fAddress;
        asm volatile ("mov %%cr2, %0" : "=r" (fAddress));
    
        String str;
        str = (r->errorCode & 0x1 ? "" : "Present");
        str += (r->errorCode & 0x2 ? " Read only" : "");
        str += (r->errorCode & 0x4 ? " User mode" : "");
        str += (r->errorCode & 0x8 ? " Reserved" : "");
        str += " @ ";
        str += String::hex(fAddress);
        
        vt << str;
        vt << "\nThread finishing.\n";
        return;
    }
    
    vt << "Thread finishing.\n";
    Task::currentThreadExits(E_UNHLD_EXCP);
}

void Thread::setState(ulong rsp, ulong rbp, ulong rip) {
    this->rsp = rsp;
    this->rbp = rbp;
    this->rip = rip;
}

void Thread::setKernelStack() {
    //dorobit
}


void Thread::sleep(uint ms) {
    state = SLEEPING;
    waitFor.time = Time::time() + ms;
    
    Task::triggerSwitch();
}

void* Thread::mkXchgSpace(uint sz) {
    if (xchSpace)
        process->heap().free(xchSpace);
    
    xchSpace = process->heap().alloc(sz);
    return xchSpace;
}

void Thread::waitIRQ(uchar irq) {
    state = IRQWAIT;
    waitFor.irq = irq;
    
    Task::triggerSwitch();
}

bool Thread::runnable() {
    if (process->getState() != RUNNING)
        return false;
    
    if (state == RUNNING)
        return true;
    
    if (state == SLEEPING && Time::time() >= waitFor.time) {
        state = RUNNING;
        return true;
    }
    return false;
}