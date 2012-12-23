#include "GDT.h"

GDT::selectorT* GDT::entry;
GDT::tssEntryT GDT::tssEntry;
GDT::ptrT* GDT::ptr;

GDT::GDT() {
    entry = new selectorT[7];
    Memory::clear(entry);
    Memory::clear(&tssEntry);
    
    ptr = new ptrT;
    ptr->base = (ulong)entry;
    ptr->limit = sizeof(selectorT) * 7 - 1;

    setGate(0x00, 0, 0, 0, 0, false, false);
    setGate(0x08, 0, 0xFFFFFFFF, 0x0A, 0, true, false);
    setGate(0x10, 0, 0xFFFFFFFF, 0x02, 0, true, false);
    setGate(0x18, 0, 0xFFFFFFFF, 0x0A, 3, true, false);
    setGate(0x20, 0, 0xFFFFFFFF, 0x02, 3, true, false);
    setGate(0x28, (ulong)&tssEntry, sizeof(tssEntry), GDT_TYPE_TSS, 0, true, true);
    
    asm volatile ("lgdt (%0) \n"
                  "mov $0x10, %%ax \n"
                  "mov %%ax, %%ds \n"
                  "mov %%ax, %%es \n"
                  "mov %%ax, %%fs \n"
                  "mov %%ax, %%gs \n"
                     
                  "pushq $0x10 \n"
                  "pushq %%rsp \n"
                  "pushfq \n"
        
                  "pushq $0x8 \n"
                  "pushq $return \n"
                  "iretq \n"
                  "return:"
    : : "r"((ulong)ptr));
    
    asm ("mov $0x28, %ax \n"
         "ltr %ax");
}

void GDT::setGate(ushort sel, ulong base, uint limit, uint type, uchar dpl, bool large, bool sys) {
    sel /= 8;
    
    entry[sel].fields.limitLow    = limit & 0xFFFF;
    entry[sel].fields.limitHigh   = (limit >> 16) & 0xF;
    entry[sel].fields.baseLow     = base & 0xFFFF;
    entry[sel].fields.baseMidLow  = (base >> 16) & 0xFF;
    entry[sel].fields.baseMidHigh = (base >> 24) & 0xFF;
    entry[sel].fields.type        = type;
    entry[sel].fields.dpl         = dpl;
    entry[sel].fields.present     = 1;
    entry[sel].fields.large       = (large ? 1 : 0);
    entry[sel].fields.opsz        = (large ? 0 : 1);
    
    if (!sys) {
        entry[sel].fields.segSel = 1;
        entry[sel].fields.granularity = 1;
    }
    
    if (sys && (type & GDT_TYPE_TSS)) {
        selectorExtT ext;
        Memory::clear(&ext);
        
        ext.fields.baseHigh = base >> 32;
        entry[sel + 1].bytes = ext.bytes;
    }
}