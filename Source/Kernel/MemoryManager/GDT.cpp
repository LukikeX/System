#include "GDT.h"

GDT::entryT* GDT::entry;
GDT::ptrT* GDT::ptr;

GDT::GDT() {
    entry = new entryT[5];
    ptr = new ptrT;
    ptr->base = (ulong)entry;
    ptr->limit = sizeof(entryT) * 5 - 1;
    
    setGate(0, 0, 0, 0, 0);
    setGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    setGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    setGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    setGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    
    asm volatile ("lgdt (%0)" :: "r"((ulong)ptr));
}

void GDT::setGate(uint p, uint base, uint limit, ushort access, uchar granularity) {
    entry[p].limit = limit & 0xFFFF;
    entry[p].granularity = (limit >> 16) & 0x0F;
    entry[p].granularity |= granularity & 0xF0;
    entry[p].access = access;
    
    entry[p].baseLow = base & 0xFFFF;
    entry[p].baseMiddle = (base >> 16) & 0xFF;
    entry[p].baseHigh = (base >> 24) & 0xFF;
}