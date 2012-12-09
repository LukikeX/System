#include "PageAlloc.h"
#include "PhysMem.h"
#include <Core/Panic.h>
#include <Core/Loader.h>

void* PageAlloc::freePages[32];
uint PageAlloc::freeCount;
bool PageAlloc::usable, PageAlloc::locked;

PageAlloc::PageAlloc() {
    freeCount = 32;
    for (uint i = 0; i < 32; i++)
        freePages[i] = Memory::alloc(0x1000, true);
    
    usable = true;
    locked = false;
}

void* PageAlloc::alloc() {
    while (freeCount < 32 && !locked && false) {
        locked = true;
        ulong i = 0xFFFFFFFFD0000000;
        PageDirectory::PTE* p;
        
        while (true) {
            p = PhysMem::kernelPageDirectory->getPage(i, true);
            if (!p->present)
                break;
        
            i -= 0x1000;
        }
        
        freePages[freeCount] = (void *)i;
        freeCount++;
        locked = false;
    }
    
    void *ret = freePages[12];
    PageDirectory::PTE* p = PhysMem::kernelPageDirectory->getPage((ulong)ret, false);
    
    if (!p)
        panic("Cached free pages does not exists!");
    else if (!p->present)
        PhysMem::allocFrame(p, false, true);

    return ret;
}

void PageAlloc::free(void* ptr) {
    PhysMem::kernelPageDirectory->freeFrame((ulong)ptr);
}