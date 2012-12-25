#ifndef TASK_H
#define TASK_H

#include "Thread.h"
#include "Process.h"
#include <Core/Panic.h>
#include <Library/List.h>
#include <VTManager/VirtualTerminal.proto.h>
#include <Library/String.h>
#include <MemoryManager/PhysMem.h>

#define INVALID_TASK_MAGIC 0xDEADC0DE

class Task {
private:
    static List<Process *>* processes;
    static List<Thread *>* threads;
    
    static List<Thread *>* currThread;
    static List<Thread *>* idleThread;
    static Process* currProcess;
    
    static uint nextpid;
    static ulong tempStack[256];
    
    static List<Thread *>* nextThread();
    static void currentThreadExitsProceed(uint errorCode);
    
public:
    Task(String cmdline, VirtualTerminal* vt);
    static Thread* currentThread() { return currThread->v(); }
    static Process* currentProcess() { return currProcess; }
    static Process* getKernelProcess();
    
    static void doSwitch();
    static inline void triggerSwitch() { asm ("int $65"); }
    static inline uint nextPid() { return nextpid++; }
    static bool IRQwakeup(uchar irq);
    static void currentThreadExits(uint errorCode);
    
    static void registerThread(Thread* t);
    static void unregisterThread(Thread* t);
    static void registerProcess(Process* p);
    static void unregisterProcess(Process* p);
};

#endif