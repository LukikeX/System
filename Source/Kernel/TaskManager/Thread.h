#ifndef THREAD_H
#define THREAD_H

#include <SyscallManager/IDT.h>

class Process;

typedef long unsigned int(*ThreadEntry)(void *);

class Thread {
    friend class Process;
protected:
    ulong rsp, rbp, rip;
    Process* process;
    
    uchar state;
    void* xchSpace;
    bool isKernel;
    
    union {
        uint time;
        uchar irq;
    } waitFor;
    
    struct {
        void* address;
        uint size;
    } userStack, kernelStack;
    
    static void run(Thread* t, void* data, ThreadEntry entryPoint);
    Thread();
    
    void setup(Process* process, ThreadEntry entryPoint, void* data, bool isKernel);
    
public:
    enum {
        ZOMBIE   = 0,
        RUNNING  = 1,
        SLEEPING = 2,
        IRQWAIT  = 3
    };
    
    Thread(ThreadEntry entryPoint, void* data, bool isKernel = false);
    Thread(Process* process, ThreadEntry entryPoint, void* data);
    ~Thread();
    void finish(uint errorCode);
    virtual void handleException(IDT::regs* r);
    
    void setState(ulong rsp, ulong rbp, ulong rip);
    void setKernelStack();
    ulong getRsp() { return rsp; }
    ulong getRbp() { return rbp; }
    ulong getRip() { return rip; }
    Process* getProcess() { return process; }
    
    void* mkXchgSpace(uint sz);
    void sleep(uint ms);
    void waitIRQ(uchar irq);
    bool runnable();
    
    inline bool IRQhappens(uchar irq) {
        if (state == IRQWAIT && waitFor.irq == irq) {
            state = RUNNING;
            return true;
        }
        return false;
    }
};

#endif