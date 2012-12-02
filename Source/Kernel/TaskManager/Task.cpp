#include "Task.h"
#include "Core/IO.h"

extern "C" long unsigned int read_rip();
extern "C" long unsigned int idle_task(void *);

List<Process *>* Task::processes;
List<Thread *>* Task::threads;
    
List<Thread *>* Task::currThread;
List<Thread *>* Task::idleThread;
Process* Task::currProcess;
    
uint Task::nextpid;
ulong Task::tempStack[256];

Task::Task(String cmdline, VirtualTerminal* vt) {
    IO::cli();
    threads = 0;
    processes = 0;
    
    currProcess = Process::createKernel(cmdline, vt);
    currThread = threads;
    
    Thread* idle = new Thread(idle_task,0 , true);
    for (List<Thread *>* iter = threads; iter; iter = iter->next()) {
        if (iter->v() == idle) {
            idleThread = iter;
            break;
        }
    }
}

Process* Task::getKernelProcess() {
    if (!processes)
        panic("Kernel process doesn't exist!");
    
    return processes->last()->v();
}

void Task::registerThread(Thread* t) {
    unregisterThread(t);
    threads = threads->cons(t);
}

void Task::unregisterThread(Thread* t) {
    if (!threads)
        return;
    
    threads = threads->removeOnce(t);
}

void Task::registerProcess(Process* p) {
    unregisterProcess(p);
    processes = processes->cons(p);
}

void Task::unregisterProcess(Process* p) {
    if (!processes)
        return;
    
    processes = processes->removeOnce(p);
}

List<Thread *>* Task::nextThread() {
    if ((ulong)currThread == INVALID_TASK_MAGIC)
        currThread = threads;
    
    List<Thread *>* iter = currThread;
    while (true) {
        iter = iter->next();
        if (!iter)
            iter = threads;
        
        if (iter->v()->runnable() && iter->v() != idleThread->v())
            return iter;
        
        if (iter == currThread)
            break;
    }
    
    return idleThread;
}

void Task::doSwitch() {
    if (!currThread || !currProcess)
        return;
    
    ulong rsp, rbp, rip;
    asm volatile ("mov %%rsp, %0" : "=r"(rsp));
    asm volatile ("mov %%rbp, %0" : "=r"(rbp));
    rip = read_rip();
    
    if (rip == 0x12345)
        return;
    
    if ((ulong)currThread != INVALID_TASK_MAGIC)
        currThread->v()->setState(rsp, rbp, rip);
    
    currThread = nextThread();
    Thread* t = currThread->v();
    currProcess = t->getProcess();
    
    rsp = t->getRsp();
    rbp = t->getRbp();
    rip = t->getRip();
    
    IO::cli();
    t->setKernelStack();
    
    asm volatile ("mov %0, %%rbp \n"
                  "mov %1, %%rsp \n"
                  "mov %2, %%rcx \n"
                  "mov $0x12345, %%rax \n"
                  "jmp *%%rcx"
                  : : "r"(rbp), "r"(rsp), "r"(rip));
}

bool Task::IRQwakeup(uchar irq) {
    bool ret = false;
    for (List<Thread *>* iter = threads; iter; iter = iter->next())
        ret = ret || iter->v()->IRQhappens(irq);
    return ret;
}

void Task::currentThreadExits(uint errorCode) {
    IO::cli();
    ulong* stack = &tempStack[255];
    *stack = errorCode;
    stack--;
    *stack = 0;
    
    ulong rsp = (ulong)stack;
    ulong rbp = (ulong)stack + 1;
    ulong rip = (ulong)currentThreadExitsProceed;
    
    asm volatile ("mov %0, %%rsp \n"
                  "mov %1, %%rbp \n"
                  "mov %2, %%rcx \n"
                  "mov %3, %%cr3 \n"
                  "jmp *%%rcx"
                  : : "r"(rsp), "r"(rbp), "r"(rip), "r"(PhysMem::kernelPageDirectory->getPhysicalAddr()));
}

void Task::currentThreadExitsProceed(uint errorCode) {
    currThread->v()->finish(errorCode);
    currThread = (List<Thread *> *)INVALID_TASK_MAGIC;
    doSwitch();
}