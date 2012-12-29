#ifndef PROCESS_H
#define PROCESS_H

#include <Library/Vector.h>
#include <MemoryManager/PageDirectory.h>
#include <VTManager/VirtualTerminal.proto.h>
#include <MemoryManager/Heap.h>
#include <SyscallManager/Ressource.h>
#include <VFS/DirectoryNode.h>
#include <VFS/File.h>

#define E_PAGEFAULT  0x0FFFFF00
#define E_EXIT       0x0FFFFF01
#define E_UNHLD_EXCP 0x0FFFFF02

#define STACKSIZE 4096

#define USERHEAPINITSIZE 0x00010000 //Heap initially is 64k, but can grow
#define USERHEAPSTART    0xB7000000 //Heap is at 0xB7000000, 128Mo before kernel space.

class Thread;

class Process : public Ressource {
    friend class Thread;
private:
    ulong pid, ppid, uid;
    Vector<String> args;
    long retval;
    uchar state;
    PageDirectory* pageDir;
    Heap* userHeap;
    VirtualTerminal* inVT, *outVT;
    DirectoryNode* cwd;
    
    bool autoDelete;
    Vector<Thread *>threads;
    List<File *> *descriptors;
    
    Process();
    
public:
    enum {
        ZOMBIE   = 0,
        RUNNING  = 1,
        STARTING = 2,
        FINISHED = 3
    };
    
    static Process* createKernel(String cmdline, VirtualTerminal* vt);
    static Process* run(String filename, ulong uid);
    Process(String binfile, ulong uid);
    ~Process();
    
    Heap& heap() { return *userHeap; }
    
    void start();
    void exit();
    void pushArg(const String& str);
    
    void registerThread(Thread* t);
    void threadFinishes(Thread* t, ulong retval);
    
    void registerFileDescriptor(File* fd);
    void unregisterFileDescriptor(File* fd);
    
    PageDirectory* getPageDir() { return pageDir; }
    ulong getUid() { return uid; }
    ulong getPid() { return pid; }
    ulong getPpid() { return ppid; }
    
    void setCwd(DirectoryNode* c) { cwd = c; }
    DirectoryNode* getCwd() { return cwd; }
    
    VirtualTerminal* getInVT() { return inVT; }
    VirtualTerminal* getOutVT() { return outVT; }
    void setInVT(VirtualTerminal* vt) { inVT = vt; }
    void setOutVT(VirtualTerminal* vt) { outVT = vt; }
    uint getState() { return state; }
    
    //Syscalls:
    static ulong scall(uint wat, ulong a, ulong, ulong, ulong);
    
private:
    static callT callTable[];
    bool accessible();
    ulong exitSC();
    ulong argcSC();
    ulong argvSC(ulong idx);
    ulong startSC();
    ulong allocPagesSC(ulong pos, ulong end);
    ulong freePagesSC(ulong pos, ulong end);
    ulong autoDeleteSC(ulong d);
    ulong pushArgSC(ulong arg);
    ulong setOutVTSC(ulong vtid);
    ulong setInVTSC(ulong vtid);
};

#endif