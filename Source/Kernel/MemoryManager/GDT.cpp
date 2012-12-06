#include "GDT.h"

GDT::entryT* GDT::entry;
GDT::tssEntryT GDT::tssEntry;
GDT::ptrT* GDT::ptr;

GDT::GDT() {
    entry = new entryT[6];
    ptr = new ptrT;
    ptr->base = (ulong)entry;
    ptr->limit = sizeof(entryT) * 6 - 1;
    
    setGate(0, 0, 0, 0, 0);
    setGate(1, 0, 0xFFFFFFFF, 0x9A, 0x20); //0x20 = 0xCF
    setGate(2, 0, 0xFFFFFFFF, 0x92, 0x20);
    setGate(3, 0, 0xFFFFFFFF, 0xFA, 0x20);
    setGate(4, 0, 0xFFFFFFFF, 0xF2, 0x20);
    writeTSS(5, 0x10, 0x00);
    
    asm volatile ("lgdt (%0)" :: "r"((ulong)ptr));
    //asm ("ltr 0x2B");
}

void GDT::setGate(uint p, uint base, uint limit, ushort access, uchar granularity) {
    entry[p].baseLow      = base & 0xFFFF;
    entry[p].baseMiddle   = (base >> 16) & 0xFF;
    entry[p].baseHigh     = (base >> 24) & 0xFF;
    entry[p].limit        = limit & 0xFFFF;
    entry[p].granularity  = (limit >> 16) & 0x0F;
    entry[p].granularity |= granularity & 0xF0;
    entry[p].access       = access;
}

void GDT::writeTSS(uint p, ushort ss0, ulong rsp0) {
    ulong base = (ulong)&tssEntry;
    ulong limit = base + sizeof(tssEntryT);
    
    setGate(p, base, limit, 0xE9, 0x00);
    Memory::clear(&tssEntry);
    
    tssEntry.ss0 = ss0;
    tssEntry.rsp0 = rsp0;
    tssEntry.iomap_base = sizeof(tssEntryT) - 8192;
    tssEntry.cs = 0x0B;
    tssEntry.ss = tssEntry.ds = tssEntry.es = tssEntry.fs = tssEntry.gs = 0x13;
}