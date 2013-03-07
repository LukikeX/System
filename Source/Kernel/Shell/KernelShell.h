#ifndef KERNELSHELL_H
#define KERNELSHELL_H

#include <TaskManager/Thread.h>
#include <VTManager/VirtualTerminal.proto.h>
#include <VFS/DirectoryNode.h>
#include <Library/Vector.h>

class KernelShell {
    friend ulong shellRun(void* ks);
private:
    static ushort instances;
    VirtualTerminal* vt;
    DirectoryNode* cwd;
    Thread* thread;
    
    ~KernelShell();
    ulong run();
    void setup(DirectoryNode* cwd, VirtualTerminal* vt);
    
    void dir(Vector<String>& args);
    void cd(Vector<String>& args);
    void pwd(Vector<String>&);
    void cat(Vector<String>& args);
    void mkdir(Vector<String>& args);
    void rm(Vector<String>& args);
    void wf(Vector<String>& args);
    void run(Vector<String>& args);
    
public:
    KernelShell(DirectoryNode* cwd);
    KernelShell(DirectoryNode* cwd, VirtualTerminal* vt);
    
    static inline ushort getInstances() { return instances; }
};

#endif