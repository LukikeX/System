#ifndef PHYSMEM_H
#define PHYSMEM_H

#include <Library/Bitset.h>
#include "PageDirectory.h"

class PhysMem {
private:
    static uint nFrames;
    static Bitset* frames;
    
public:
    static PageDirectory* kernelPageDirectory;
    
    PhysMem();
    
    static void allocFrame(PageDirectory::PTE* page, bool isUser, bool isWritable);
    static void freeFrame(PageDirectory::PTE* page);
    
    static uint free() { return nFrames - frames->usedBits(); }
    static uint total() { return nFrames; }
    static void getMemoryMap();
};

#endif