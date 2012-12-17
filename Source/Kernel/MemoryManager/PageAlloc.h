#ifndef PAGEALLOC_H
#define PAGEALLOC_H

#include <Core/typedef.h>

class PageAlloc {
private:
    static void* freePages[32];
    static uint freeCount, pointer;
    static bool usable, locked;
    
public:
    PageAlloc();
    static void* alloc();
    static void free(void* ptr);
};

#endif