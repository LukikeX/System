#include "System.h"
#include <SyscallManager/Ressource.h>

ulong System::scall(uint wat, ulong, ulong, ulong, ulong) {
    if (wat == SYIF_SREBOOT)
        reboot();
    
    return 0;
}

void System::reboot() {
    uchar temp;
    IO::cli();
 
    do {
        temp = IO::inB(KBRD_INTRFC);
        if (checkFlag(temp, KBRD_BIT_KDATA))
            IO::inB(KBRD_IO);
    } while (checkFlag(temp, KBRD_BIT_UDATA));
 
    IO::outB(KBRD_INTRFC, KBRD_RESET);
    for (;;);
}