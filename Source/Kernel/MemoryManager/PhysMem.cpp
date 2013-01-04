#include "PhysMem.h"
#include "Memory.h"
#include <Exceptions/MemoryException.h>
#include <Core/Loader.h>

uint PhysMem::nFrames;
Bitset* PhysMem::frames;
PageDirectory* PhysMem::kernelPageDirectory;

PhysMem::PhysMem() {
    nFrames = 0x20000;
    frames = new Bitset(nFrames);
    
    kernelPageDirectory = new PageDirectory();
    
    for (ulong i = 0xFFFFFFFFC0000000; i < 0xFFFFFFFFC0800000; i += 0x1000)
        kernelPageDirectory->allocFrame(i, false, true);

    kernelPageDirectory->switchTo();
}

void PhysMem::allocFrame(PageDirectory::PTE* page, bool isUser, bool isWritable) {
    if (page->present)
        throw new MemoryException("Page is already mapped!");
    else {
        uint idx = frames->firstFreeBit();
        
        if (idx == (uint)-1)
            throw new MemoryException("No more free frames!");
        
        frames->setBit(idx);
        page->present = 1;
        page->user = (isUser ? 1 : 0);
        page->readWrite = (isWritable ? 1 : 0);
        page->address = idx;
    }
}

void PhysMem::freeFrame(PageDirectory::PTE* page) {
    if (!page->present)
        throw new MemoryException("Page is not mapped!");
    else {
        if (page->address >= 0x100)
            frames->clearBit(page->address);
        
        page->present = 0;
        page->address = 0;
    }
}

void PhysMem::getMemoryMap() {
    for (uint i = 0; i < 1700; i++) {
        bool n = false;
        for (uint j = 0; j < PhysMem::nFrames / 1700; j++) {
            if (PhysMem::frames->testBit(PhysMem::nFrames / 1700 * i + j)) {
                n = true;
                break;
            }
        }
        
        if (n) {
            kvt->setColor(2 << 4);
            *kvt << " ";
        } else {
            kvt->setColor(7 << 4);
            *kvt << " ";
        }
    }
    kvt->setColor(7);
}