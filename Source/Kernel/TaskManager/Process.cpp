#include "Process.h"
#include "Thread.h"
#include "Task.h"
#include "VFS/File.h"

Process::Process() : Ressource(PRIF_OBJTYPE, callTable) { }

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

Process* Process::run(String filename, ulong uid) {
  //  File file(filename, File::FM_READ, (FSNode *)Task::currentProcess()->getCwd());
  //  if (!file.isValid())
 //       return 0;
    
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

Process::Process(String binfile, ulong uid) : Ressource(PRIF_OBJTYPE, callTable) {
    pid = Task::nextPid();
    ppid = Task::currentProcess()->getPid();
    args.push(binfile);
    retval = 0;
    state = STARTING;
    this->uid = uid;
    autoDelete = false;
    cwd = Task::currentProcess()->getCwd();
    inVT = Task::currentProcess()->getInVT();
    outVT = Task::currentProcess()->getOutVT();
    descriptors = 0;
    
    pageDir = new PageDirectory(PhysMem::kernelPageDirectory);
    pageDir->switchTo();
    userHeap = new Heap();
    
    uint heapIdxSize = PhysMem::total() * 16 + 10000;
    userHeap->create(USERHEAPSTART, USERHEAPINITSIZE + heapIdxSize, heapIdxSize, pageDir, true, true);
    Task::registerProcess(this);
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
    while (descriptors) {
        //descriptors->v()->close();
        delete descriptors->v();
        descriptors = descriptors->deleteThis();
    }
    state = FINISHED;
}

void Process::registerThread(Thread* t) {
    if (state != FINISHED)
        threads.push(t);
}

void Process::threadFinishes(Thread* t, ulong retval) {
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

void Process::registerFileDescriptor(File* fd) {
    descriptors = descriptors->cons(fd);
}

void Process::unregisterFileDescriptor(File* fd) {
    descriptors = descriptors->removeOnce(fd);
}