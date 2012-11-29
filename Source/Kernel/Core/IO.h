#ifndef IO_H
#define IO_H

#include "typedef.h"

class IO {
public:
    typedef ushort port;
    
    inline static void outB(port p, uchar data) { asm volatile ("outb %1, %0" : : "dN" (p), "a" (data)); }
    inline static void outW(port p, ushort data) { asm volatile ("outw %1, %0" : : "dN" (p), "a" (data)); }
    inline static void outL(port p, uint data) { asm volatile ("outl %1, %0" : : "dN" (p), "a" (data)); }
    
    static uchar inB(port p);
    static ushort inW(port p);
    static uint inL(port p);
    
    static void is(port p, uchar* data, uint count);
    static void is(port p, ushort* data, uint count);
    static void is(port p, uint* data, uint count);
    
    static void cpuid(int code, uint* a, uint* d) { asm volatile("cpuid" : "=a" (*a), "=d" (*d) : "a" (code) : "rcx", "rbx"); }
    
    inline static void cli() { asm volatile ("cli"); }
    inline static void sti() { asm volatile ("sti"); }
};

#endif