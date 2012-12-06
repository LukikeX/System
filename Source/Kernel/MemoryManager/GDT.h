#ifndef GDT_H
#define GDT_H

#include <Core/typedef.h>
#include <MemoryManager/Memory.h>

class GDT {
private:
    struct entryT {
        ushort limit;
        ushort baseLow;
        uchar  baseMiddle;
        uchar  access;
        uchar  granularity;
        uchar  baseHigh;
    } __attribute__((packed));
    
    struct ptrT {
        ushort limit;
        ulong  base;
    } __attribute__((packed));
    
    static entryT* entry;
    static ptrT*   ptr;
    
    static void setGate(uint p, uint base, uint limit, ushort access, uchar granularity);
    static void writeTSS(uint p, ushort ss0, ulong rsp0);
    
public:
    struct tssEntryT {
        ulong prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
        ulong rsp0;       // The stack pointer to load when we change to kernel mode.
        uint ss0;        // The stack segment to load when we change to kernel mode.
        ulong rsp1;       // Unused...
        uint ss1;
        ulong rsp2;
        uint ss2;
        ulong cr3;
        ulong rip;
        ulong rflags;
        ulong rax;
        ulong rcx;
        ulong rdx;
        ulong rbx;
        ulong rsp;
        ulong rbp;
        ulong rsi;
        ulong rdi;
        uint es;         // The value to load into ES when we change to kernel mode.
        uint cs;         // The value to load into CS when we change to kernel mode.
        uint ss;         // The value to load into SS when we change to kernel mode.
        uint ds;         // The value to load into DS when we change to kernel mode.
        uint fs;         // The value to load into FS when we change to kernel mode.
        uint gs;         // The value to load into GS when we change to kernel mode.
        uint ldt;        // Unused...
        ushort trap;
        ushort iomap_base;
        uchar ioports[8192];
    } __attribute__((packed));
        
    static tssEntryT tssEntry;
    
    GDT();
};

#endif