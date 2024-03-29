#include "Thread.h"
#include "Task.h"
#include <UserManager/UserManager.h>
#include <Core/IO.h>
#include <DeviceManager/Time.h>
#include <MemoryManager/GDT.h>

Thread::callT Thread::callTable[] = {
    CALL1(THIF_SLEEP,  &Thread::sleepSC),
    CALL1(THIF_FINISH, &Thread::finishSC),
    CALL0(0, 0)
};

ulong Thread::scall(uint wat, ulong, ulong, ulong, ulong) {
    if (wat == THIF_SGETCTH)
        return Task::currentThread()->resId();
    
    return (ulong)-1;
}

void Thread::run() {
    IO::cli();
    Thread* t;
    void* data;
    ThreadEntry entryPoint;
    
    asm volatile ("mov 16(%%rbp), %0 \n"
                  "mov 24(%%rbp), %1 \n"
                  "mov 32(%%rbp), %2 \n"
                  : "=r"((ulong)t), "=r"((ulong)data), "=r"((ulong)entryPoint)
    );
    
    if (t->isKernel) {
        IO::sti();
        asm volatile ("int $66" : : "a"(entryPoint(data)));
    } else {
        ulong* stack = (ulong *)((ulong)t->userStack.address + t->userStack.size);
        stack--;
        *stack = (ulong)data;
        stack--;
        *stack = 0;
                
        asm volatile ("mov $0x23, %%ax \n"
                      "mov %%ax, %%ds \n"
                      "mov %%ax, %%es \n"
                      "mov %%ax, %%fs \n"
                      "mov %%ax, %%gs \n"
                      
                      "pushq $0x23 \n"
                      "pushq %0 \n"
        
                      "pushfq \n"
                      "pop %%rax \n"
                      "or $0x200, %%rax \n"
                      "pushq %%rax \n"
        
                      "pushq $0x1B \n"
                      "pushq %1 \n"
                      "iretq"
                      : : "r"((ulong)stack), "r"((ulong)entryPoint)
        );
    }
}

Thread::Thread() : Ressource(THIF_OBJTYPE, callTable) {
    xchSpace = 0;
}

Thread::Thread(ThreadEntry entryPoint, void* data, bool isKernel) : Ressource(THIF_OBJTYPE, callTable) {
    if (isKernel)
        setup(Task::getKernelProcess(), entryPoint, data, true);
    else
        setup(Task::currentProcess(), entryPoint, data, false);
}

Thread::Thread(Process* process, ThreadEntry entryPoint, void* data) : Ressource(THIF_OBJTYPE, callTable) {
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

    ulong* stack = (ulong *)((ulong)kernelStack.address) + kernelStack.size;
    rbp  = (ulong)stack;
    stack--;
    *stack = (ulong)entryPoint;
    stack--;
    *stack = (ulong)data;
    stack--;
    *stack = (ulong)this;
    stack--;
    *stack = 0;
    
    rsp = (ulong)stack;
    rip = (ulong)run;
    state = RUNNING;
    
    process->registerThread(this);
    Task::registerThread(this);
}

void Thread::finish(uint errorCode) {
    if (errorCode == E_PAGEFAULT && isKernel)
        panic("Page fault in kernel thread!");
    
    if (errorCode == E_UNHLD_EXCP && isKernel)
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
    vt << "\nUnhandled exception " << (int)r->intNo << " at " << (ushort)r->cs << ":"
       << r->rip << "\n:: " << exceptions[r->intNo];
    
    if (r->intNo == 3) {
        vt << "\n\nBreakpoint data:\n";
        dumpRegisters(r, &vt);
        vt << "Press any key to continue execution of program...";
        process->getInVT()->getkeyPress(false);
        vt << "\n";
        return;
    }
    for (;;);
    if (isKernel) {
        if (r->intNo == 14) {
            ulong fAddress;
            asm volatile ("mov %%cr2, %0" : "=r" (fAddress));

            String str = " |";
            str += (r->errorCode & 0x1 ? "" : " present");
            str += (r->errorCode & 0x2 ? " read only" : "");
            str += (r->errorCode & 0x4 ? " user mode" : "");
            str += (r->errorCode & 0x8 ? " reserved" : "");
            str += " @ ";
            str += String::hex(fAddress);

            vt << str;
            IO::cli();
            for (;;);
        }
        panic(exceptions[r->intNo], r);
    }
    
    if (r->intNo == 14) {
        ulong fAddress;
        asm volatile ("mov %%cr2, %0" : "=r" (fAddress));
    
        String str = " |";
        str += (r->errorCode & 0x1 ? "" : " present");
        str += (r->errorCode & 0x2 ? " read only" : "");
        str += (r->errorCode & 0x4 ? " user mode" : "");
        str += (r->errorCode & 0x8 ? " reserved" : "");
        str += " @ ";
        str += String::hex(fAddress);
        
        vt << str;
        vt << "\nThread finishing.\n";
        Task::currentThreadExits(E_PAGEFAULT);
        return;
    }
    
    vt << "\nThread finishing.\n";
    Task::currentThreadExits(E_UNHLD_EXCP);
}

void Thread::setState(ulong rsp, ulong rbp, ulong rip) {
    this->rsp = rsp;
    this->rbp = rbp;
    this->rip = rip;
}

void Thread::setKernelStack() {
    GDT::setTSSRSP((ulong)kernelStack.address + kernelStack.size);
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
    if (process->getState() != Process::RUNNING)
        return false;
    
    if (state == RUNNING)
        return true;
    
    if (state == SLEEPING && Time::time() >= waitFor.time) {
        state = RUNNING;
        return true;
    }
    return false;
}

ulong Thread::sleepSC(ulong ms) {
    if (this != Task::currentThread())
        return 1;
    sleep(ms);
    return 0;
}

ulong Thread::finishSC(ulong errCode) {
    if (this != Task::currentThread())
        return 1;
    Task::currentThreadExits(errCode);
    return 0;
}

bool Thread::accessible() {
    return UserManager::uid() == process->uid;
}