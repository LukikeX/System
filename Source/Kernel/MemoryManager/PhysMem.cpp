#include "PhysMem.h"
#include "Memory.h"
#include <Exceptions/MemoryException.h>
#include <Core/Loader.h>

uint PhysMem::nFrames;
Bitset* PhysMem::frames;
PageDirectory* PhysMem::kernelPageDirectory;

PhysMem::PhysMem(ulong size) {
    nFrames = 0x60000;
    frames = new Bitset(nFrames);
    
    kernelPageDirectory = new PageDirectory();
    
    for (ulong i = 0xFFFFFFFFC0000000; i < 0xFFFFFFFFC0800000; i += 0x1000)
        kernelPageDirectory->allocFrame(i, false, true);

    kernelPageDirectory->switchTo();

    nFrames = size;
    frames->resize(nFrames);
}

void PhysMem::allocFrame(PageDirectory::PTE* page, bool isUser, bool isWritable) {
    if (page->present)
        return;//throw new MemoryException("Page is already mapped!");
    else {
        uint idx = frames->firstFreeBit();
        
        if (idx == (uint)-1)
            return; //throw new MemoryException("No more free frames!");
        
        frames->setBit(idx);
        page->present = 1;
        page->user = (isUser ? 1 : 0);
        page->readWrite = (isWritable ? 1 : 0);
        page->address = idx;
    }
}

void PhysMem::freeFrame(PageDirectory::PTE* page) {
    if (!page->present)
        return; //throw new MemoryException("Page is not mapped!");
    else {
        if (page->address >= 0x100)
            frames->clearBit(page->address);
        
        page->present = 0;
        page->address = 0;
    }
}

void PhysMem::getMemoryMap() {
    uint block = PhysMem::nFrames / 1700;
    for (uint i = 0; i < PhysMem::nFrames; i += block) {
        bool n = false;
        for (uint j = 0; j < block; j++) {
            if (PhysMem::frames->testBit(i + j)) {
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