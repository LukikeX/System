#include "Memory.h"

Heap Memory::kernelHeap;
ulong Memory::placementAddress;

Memory::Memory() {
    ulong indexSize = PhysMem::total() / 0x1000 * 64 + 0x10000;
    ulong start = (placementAddress & 0xFFFFFFFFFFFFF000) + 0x10000;
    ulong size = HEAP_MIN_SIZE + indexSize;
    
    kernelHeap.create(start, size, indexSize, PhysMem::kernelPageDirectory, false, true);
}

void* Memory::allocInternal(ulong size) {
    if (kernelHeap.usable())
        return 0;
    
    placementAddress &= 0xFFFFFFFFFFFFF000;
    placementAddress += 0x1000;
    ulong temp = placementAddress;
    placementAddress += size;
    return (void *)temp;
}

void* Memory::alloc(ulong size) {
    if (!kernelHeap.usable())
        return allocInternal(size);
    return kernelHeap.alloc(size);
}

void Memory::free(void* p) {
    kernelHeap.free(p);
}

void* Memory::mkXchgSpace(ulong) {
    return (void *)0; //task->CurrentThread()->mkXchgSpace(Size);
}
