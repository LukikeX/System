#include "IO.h"

uchar IO::inB(port p) {
    uchar ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (p));
    return ret;
}

ushort IO::inW(port p) {
    ushort ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (p));
    return ret;
}

uint IO::inL(port p) {
    uint ret;
    asm volatile ("inl %%dx, %%eax" : "=a" (ret) : "d" (p));
    return ret;
}

void IO::is(port p, uchar* data, uint count) {
    while (count--)
        *data++ = inB(p);
}

void IO::is(port p, ushort* data, uint count) {
    while (count--)
        *data++ = inW(p);
}

void IO::is(port p, uint* data, uint count) {
    while (count--)
        *data++ = inL(p);
}