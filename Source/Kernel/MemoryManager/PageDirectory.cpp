#include "PageDirectory.h"
#include "PhysMem.h"
#include "Memory.h"
#include "PageAlloc.h"

PageDirectory::PageDirectory() {
    pages = (PML4 *)Memory::alloc(sizeof(PML4), true);
    Memory::clear(pages);
}

PageDirectory::PageDirectory(const PageDirectory& other) {
    pages = (PML4 *)Memory::alloc(sizeof(PML4), true);
    Memory::clear(pages);
    
    for (ulong i = 0; i < 512; i++) {
        if (other.pages->entries[i].present) {
            for (ulong j = 0; j < 512; j++) { 
                if (other.pages->tables[i]->entries[j].address) {
                    for (ulong k = 0; k < 512; k++) {
                        if (other.pages->tables[i]->tables[j]->entries[k].present) {
                            for (ulong m = 0; m < 512; m++) {
                                if (other.pages->tables[i]->tables[j]->tables[k]->entries[m].present) {
                                    PTE* pres = &other.pages->tables[i]->tables[j]->tables[k]->entries[m];
                                    
                                    ulong virtualAddress = (i << 39) | (j << 30) | (k << 21) | (m << 12);  
                                    PTE* pte = getPage(virtualAddress, true);
                                    map(pte, pres->address << 12, pres->user, pres->readWrite);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#include <Core/Loader.h>
PageDirectory::PageDirectory(PageDirectory* other) {
    pages = (PML4 *)Memory::alloc(sizeof(PML4), true);
    Memory::clear(pages);
    
    for (ulong i = 0; i < 512; i++) {
        if (other->pages->entries[i].present) {
            for (ulong j = 0; j < 512; j++) { 
                if (other->pages->tables[i]->entries[j].address) {
                    for (ulong k = 0; k < 512; k++) {
                        if (other->pages->tables[i]->tables[j]->entries[k].present) {
                            for (ulong m = 0; m < 512; m++) {
                                if (other->pages->tables[i]->tables[j]->tables[k]->entries[m].present) {
                                    PTE* pres = &other->pages->tables[i]->tables[j]->tables[k]->entries[m];
                                    ulong virtualAddress = (i << 39) | (j << 30) | (k << 21) | (m << 12); 
                                    PTE* p = getPage(virtualAddress, true);
                                    
                                    if (virtualAddress & 0xFFFFFFFFC0000000)
                                        map(p, pres->address << 12, pres->user, pres->readWrite);
                                    else {
                                        PhysMem::allocFrame(p, false, true);

                                        PTE* vp = PhysMem::kernelPageDirectory->getPage(0, true);
                                        PhysMem::kernelPageDirectory->map(vp, p->address * 0x1000, false, true);
                                        Memory::copy((char *)(pres->address >> 12), (char *)0, 0x1000);
                                        PhysMem::kernelPageDirectory->unmap(vp);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

PageDirectory::~PageDirectory() {
    for (uint i = 0; i < 512; i++) {
        if (pages->entries[i].present) {
            for (uint j = 0; j < 512; j++) {
                if (pages->tables[i]->entries[j].present) {
                    for (uint k = 0; k < 512; k++) {
                        if (pages->tables[i]->tables[j]->entries[k].present)
                            PageAlloc::free(pages->tables[i]->tables[j]->tables[k]);
                    }
                    PageAlloc::free(pages->tables[i]->tables[j]);
                }
            }
            PageAlloc::free(pages->tables[i]);
        }
    }
    PageAlloc::free(pages);
}

void PageDirectory::map(PTE* page, ulong physAddress, bool user, bool rw) {
    page->present   = 1;
    page->user      = (user ? 1 : 0);
    page->readWrite = (rw ? 1 : 0);
    page->address   = physAddress >> 12;
}

void PageDirectory::switchTo() {
    asm volatile ("mov %0, %%cr3" : : "r"(getPhysAddress()));
    ulong cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" : : "r"(cr0));    
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
        pdpt = (PDPT *)Memory::alloc(sizeof(PDPT), true);
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
        pd = (PD *)Memory::alloc(sizeof(PD), true);
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
        pt = (PT *)Memory::alloc(sizeof(PT), true);
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

ulong PageDirectory::getPhysAddress(ulong address) {
    ulong startPML4E = (address >> 39) & 511,
            startPDPTE = (address >> 30) & 511,
            startPDE   = (address >> 21) & 511,
            startPTE   = (address >> 12) & 511;

    PDPT* pdpt;
    if (pages->entries && pages->entries[startPML4E].present)
        pdpt = pages->tables[startPML4E];
    else
        return address & ~0xFFFFFFFFC0000000;

    PD* pd;
    if (pdpt->entries && pdpt->entries[startPDPTE].present)
        pd = pdpt->tables[startPDPTE];
    else
        return address & ~0xFFFFFFFFC0000000;

    PT* pt;
    if (pd->entries && pd->entries[startPDE].present)
        pt = pd->tables[startPDE];
    else
        return address & ~0xFFFFFFFFC0000000;

    if (!pt->entries)
        return address & ~0xFFFFFFFFC0000000;

    return (ulong)pt->entries[startPTE].address << 12;
}

ulong PageDirectory::getPhysAddress() {
    return getPhysAddress((ulong)pages);
}