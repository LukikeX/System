#ifndef V86THREAD_H
#define V86THREAD_H

#define V86_STACKSIZE 1024

#define EFLAGS_IF 0x200
#define EFLAGS_VM 0x20000
#define VALID_FLAGS 0xDFF

#include <Core/typedef.h>
#include <SyscallManager/IDT.h>
#include <TaskManager/Thread.h>

class V86Thread : public Thread {
public:
    struct functionT {
        ushort size;
        uchar data[];
    } __attribute__((packed));
    
    struct regsT {
        ushort ax, bx, cx, dx, di, si;
        ushort cs, ds, es, fs, gs, ss;
        ushort ip, sp;
    };
    
    struct retvalT {
        regsT* regs;
        bool finished;
    };
private:
    V86Thread();
    V86Thread(V86Thread&);
    void setup();
    
    retvalT* ret;
    bool m_if, continueOnIret;
    
public:
    V86Thread(functionT* entry, retvalT* ret);
    V86Thread(uchar intNo, retvalT* ret);
    
    bool handleV86GPF(IDT::regs* r);
    void handleException(IDT::regs* r);
};

#endif