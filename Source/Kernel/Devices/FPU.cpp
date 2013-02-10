#include "FPU.h"

FPU::FPU() {
    ulong cr4;
    asm volatile ("mov %%cr4, %0" : "=r"(cr4));
    cr4 |= 0x200;
    asm volatile ("mov %0, %%cr4" :: "r"(cr4));
    setCW(0x37F);
}

void FPU::setCW(const ushort cw) {
    asm volatile ("fldcw %0" :: "m"(cw));
}