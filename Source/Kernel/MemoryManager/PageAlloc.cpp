#include "PageAlloc.h"
#include "PhysMem.h"
#include <Core/Panic.h>

void* PageAlloc::freePages[32];
uint PageAlloc::freeCount, PageAlloc::pointer;
bool PageAlloc::usable, PageAlloc::locked;

PageAlloc::PageAlloc() {
    freeCount = 32;
    pointer   = 0;
    usable    = true;
    locked    = false;
    
    for (uint i = 0; i < 32; i++)
        freePages[i] = Memory::alloc(0x1000, true);
}

void* PageAlloc::alloc() {
    while (freeCount < 16 && !locked) {
        locked = true;
        ulong i = 0xFFFFFFFFFFFFF000;
        PageDirectory::PTE* p;
        
        for (uint j = 0; j < 16; j++) {
            while (i) {
                p = PhysMem::kernelPageDirectory->getPage(i, true);
                if (!p->present)
                    break;

                i -= 0x1000;
                freePages[freeCount++] = (void *)i;
            }
        }
        locked = false;
    }
    
    void *ret = freePages[pointer++];
    PageDirectory::PTE* p = PhysMem::kernelPageDirectory->getPage((ulong)ret, false);
    
    if (pointer == 32)
        pointer = 0;
    freeCount--;
    
    if (!p)
        panic("Cached free pages does not exists!");
    else if (!p->present)
        PhysMem::allocFrame(p, false, true);

    return ret;
}

void PageAlloc::free(void* ptr) {
    PhysMem::kernelPageDirectory->freeFrame((ulong)ptr);
}