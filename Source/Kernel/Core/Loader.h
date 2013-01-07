#ifndef LOADER_H
#define LOADER_H

#include <VTManager/SimpleVT.h>
#include "typedef.h"

extern SimpleVT* kvt;
extern void (*_end)();

struct memoryMap_T {
    ulong base;
    ulong length;
    uint  type;
    uint  exAddr;
} __attribute__((__packed__));

struct header_T {
    ushort       mapLen;
    memoryMap_T* memMap;
} __attribute__((__packed__));


#endif