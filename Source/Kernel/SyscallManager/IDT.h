#ifndef IDT_H
#define	IDT_H

#include <Core/typedef.h>

class IDT {
private:
    struct entryT {
        ushort baseLow;
        ushort sel;
        uchar  always0;
        uchar  flags;
        ushort baseMiddle;
        uint   baseHigh;
        uint   always02;
    } __attribute__((packed));
    
    struct ptrT {
        ushort limit;
        ulong  base;
    } __attribute__((packed));
    
    static entryT* entries;
    static ptrT* ptr;
    
public:
    struct regs {
        ulong ds;
        ulong rax, rbx, rcx, rdx, rsp, rbp, rsi, rdi;
        ulong intNo, errorCode;
        ulong rip, cs, flags, useresp, ss;
    };
    
    IDT();
    static void setGate(uint num, ulong base, ushort sel, uint flags);
    static void handler(regs* r);
};

#endif