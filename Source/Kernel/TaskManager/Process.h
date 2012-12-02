#ifndef PROCESS_H
#define PROCESS_H

#include <Library/Vector.h>
#include <MemoryManager/PageDirectory.h>
#include <VTManager/VirtualTerminal.proto.h>
#include <MemoryManager/Heap.h>

#define E_PAGEFAULT  0x0FFFFF00
#define E_EXIT       0x0FFFFF01
#define E_UNHLD_EXCP 0x0FFFFF02

class Thread;

class Process {
    friend class Thread;
private:
    uint pid, ppid, uid;
    Vector<String> args;
    int retval;
    uchar state;
    PageDirectory* pageDir;
    Heap* userHeap;
    VirtualTerminal* inVT, *outVT;
    //Directory node... *cwd
    
    bool autoDelete;
    Vector<Thread *>threads;
    //list file * file descriptors
    
    Process() { }
    
public:
    enum {
        ZOMBIE   = 0,
        RUNNING  = 1,
        STARTING = 2,
        FINISHED = 3
    };
    
    static Process* createKernel(String cmdline, VirtualTerminal* vt);
    static Process* run(String filename, uint uid);
    Process(String binfile, uint uid);
    ~Process();
    
    Heap& heap() { return *userHeap; }
    
    void start();
    void exit();
    void pushArg(const String& str);
    
    void registerThread(Thread* t);
    void threadFinishes(Thread* t, uint retval);
    
    //register & unregister file desc
    
    PageDirectory* getPageDir();
    uint getUid() { return uid; }
    uint getPid() { return pid; }
    uint getPpid() { return ppid; }
    
    //set cwd...
    
    VirtualTerminal* getInVT();
    VirtualTerminal* getOutVT();
    void setInVT(VirtualTerminal* vt);
    void setOutVT(VirtualTerminal* vt);
    uint getState() { return state; }
};


#endif