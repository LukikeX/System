#ifndef V86THREAD_H
#define V86THREAD_H

#include <Core/typedef.h>
#include <SyscallManager/IDT.h>

class V86Thread {
public:
    struct functionT {
        ushort size;
        uchar data[];
    };
    
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