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
    
public:
    GDT();
};

#endif