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

Process* Process::run(String filename, uint uid) {
    //file...
    //bin load...
    Process* p = new Process(filename, uid);
    //threadEntry b->toprocess....
    ThreadEntry e = (ThreadEntry)0;
    //delete b;
    
    if (e) {
        new Thread(p, e, 0);
        return p;
    } else {
        delete p;
        return 0;
    }
}

Process::Process(String binfile, uint uid) {
    pid = Task::nextPid();
    ppid = Task::currentProcess()->getPid();
    args.push(binfile);
    retval = 0;
    state = STARTING;
    this->uid = uid;
    autoDelete = false;
    //set cwd
    inVT = Task::currentProcess()->getInVT();
    outVT = Task::currentProcess()->getOutVT();
    //set file desc
    
    pageDir = new PageDirectory(PhysMem::kernelPageDirectory);
    //pageDir->switchTo();
    //userHeap = new Heap();
    
    //*kvt << (ulong)userHeap << "x";
    //uint heapIdxSize = PhysMem::total() * 16 + 10000;
    return;
   // uint heapIdxSize = 50000;
   // userHeap->create(USERHEAPSTART, USERHEAPINITSIZE + heapIdxSize, heapIdxSize, pageDir, true, true);
   // return;
   // Task::registerProcess(this);
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
    //add file descriptors
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