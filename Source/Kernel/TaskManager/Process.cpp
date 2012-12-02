#include "Process.h"
#include "Thread.h"
#include "Task.h"

Process* Process::createKernel(String cmdline, VirtualTerminal* vt) {
    Process* p = new Process();
    p->pid = 0;
    p->ppid = 0;
    p->args = cmdline.split(" ");
    p->retval = 0;
    p->state = RUNNING;
    p->pageDir = PhysMem::kernelPageDirectory;
    p->uid = 0;
    p->userHeap = &Memory::kernelHeap;
    p->inVT = p->outVT = vt;
    
    Thread* t = new Thread();
    t->process = p;
    t->state = Thread::RUNNING;
    t->isKernel = true;
    t->kernelStack.address = 0;
    t->kernelStack.size = 0;
    
    p->registerThread(t);
    Task::registerProcess(p);
    Task::registerThread(t);
    
    return p;
}

Process::~Process() {
    exit();
    delete pageDir;
    delete userHeap;
    Task::unregisterProcess(this);
}

void Process::start() {
    if (state == STARTING)
        state = RUNNING;
}

void Process::pushArg(const String& str) {
    args.push(str);
}

void Process::exit() {
    for (uint i = 0; i < threads.size(); i++)
        delete threads[i];
    
    threads.clear();
    
    
    state = FINISHED;
}

void Process::registerThread(Thread* t) {
    if (state != FINISHED)
        threads.push(t);
}

void Process::threadFinishes(Thread* t, uint retval) {
    if (t == threads[0] || retval == E_PAGEFAULT || retval == E_EXIT) {
        this->retval = retval;
        if (autoDelete)
            delete this;
        else
            exit();
    } else {
        for (uint i = 0; i < threads.size(); i++) {
            if (threads[i] == t) {
                threads[i] = threads.back();
                threads.pop();
                break;
            }
            delete t;
        }
    }
}