#include "PageDirectory.h"
#include "PhysMem.h"
#include "Memory.h"

PageDirectory::PageDirectory() {
    pages = new PML4();
    Memory::clear(pages);
}

void PageDirectory::map(PTE* page, ulong physAddress, bool user, bool rw) {
    page->present = 1;
    page->user= (user ? 1 : 0);
    page->readWrite = (rw ? 1 : 0);
    page->address = physAddress >> 12;
}

void PageDirectory::switchTo() {
    asm volatile ("mov %0, %%cr3" : : "r"(getPhysAddress((ulong)pages)));
}

PageDirectory::PTE* PageDirectory::getPage(ulong virtualAddress, bool make) {
    ulong startPML4E = (virtualAddress >> 39) & 511,
          startPDPTE = (virtualAddress >> 30) & 511,
          startPDE   = (virtualAddress >> 21) & 511,
          startPTE   = (virtualAddress >> 12) & 511;
    
    PDPT* pdpt;
    if (pages->entries[startPML4E].present)
        pdpt = pages->tables[startPML4E];
    else if (make) {
        pdpt = new PDPT();
        Memory::clear(pdpt);
        pages->entries[startPML4E].present = 1;
        pages->entries[startPML4E].readWrite = 1;
        pages->entries[startPML4E].address = getPhysAddress((ulong)pdpt) >> 12;
        pages->tables[startPML4E] = pdpt;
    } else
        return 0;
    
    PD* pd;
    if (pdpt->entries[startPDPTE].present)
        pd = pdpt->tables[startPDPTE];
    else if (make) {
        pd = new PD();
        Memory::clear(pd);
        pdpt->entries[startPDPTE].present = 1;
        pdpt->entries[startPDPTE].readWrite = 1;
        pdpt->entries[startPDPTE].address = getPhysAddress((ulong)pd) >> 12;
        pdpt->tables[startPDPTE] = pd;
    } else
        return 0;
    
    PT* pt;
    if (pd->entries[startPDE].present)
        pt = pd->tables[startPDE];
    else if (make) {
        pt = new PT();
        Memory::clear(pt);
        pd->entries[startPDE].present = 1;
        pd->entries[startPDE].readWrite = 1;
        pd->entries[startPDE].address = getPhysAddress((ulong)pt) >> 12;
        pd->tables[startPDE] = pt;
    } else
        return 0;
    
    return &pt->entries[startPTE];
}

void PageDirectory::allocFrame(ulong address, bool isUser, bool isWritable) {
    PTE* p = getPage(address, true);
    PhysMem::allocFrame(p, isUser, isWritable);
}

void PageDirectory::freeFrame(ulong address) {
    PTE* p = getPage(address, false);
    if (!p)
        return;
    PhysMem::freeFrame(p);
    
}