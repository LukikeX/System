#ifndef SYSTEM_H
#define SYSTEM_H

#include "IO.h"

#define KBRD_INTRFC    0x64
#define KBRD_BIT_KDATA 0
#define KBRD_BIT_UDATA 1
#define KBRD_IO        0x60
#define KBRD_RESET     0xFE
 
#define bit(n) (1 << n)
#define checkFlag(flags, n) (flags & bit(n))


class System {
public:
    static ulong scall(uint wat, ulong, ulong, ulong, ulong);
    
    static void reboot();
};

#endif