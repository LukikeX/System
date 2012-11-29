#ifndef PAGEDIRECTORY_H
#define PAGEDIRECTORY_H

#include <Core/typedef.h>

class PageDirectory {
public:
    struct PTE {
        ulong 
            present     :1,
            readWrite   :1,
            user        :1,
            writeThrough:1,
            cacheDisable:1,
            accessed    :1,
            dirty       :1,
            PAT         :1,
            global      :1,
            ignored     :3,
            address     :40,
            ignored2    :11,
            reserved    :1;
    };
private:
    struct PDE {
        ulong
            present     :1,
            readWrite   :1,
            user        :1,
            writeThrough:1,
            cacheDisable:1,
            accessed    :1,
            ignored     :1,
            reserved    :1,
            ignored2    :4,
            address     :40,
            ignored3    :11,
            reserved2   :1;     
    };
    
    struct PDPTE {
        ulong
            present     :1,
            readWrite   :1,
            user        :1,
            writeThrough:1,
            cacheDisable:1,
            accessed    :1,
            ignored     :1,
            reserved    :1,
            ignored2    :4,
            address     :40,
            ignored3    :11,
            reserved2   :1;      
    };
    
    struct PML4E {
        ulong
            present     :1,
            readWrite   :1,
            user        :1,
            writeThrough:1,
            cacheDisable:1,
            accessed    :1,
            ignored     :1,
            reserved    :1,
            ignored2    :4,
            address     :40,
            ignored3    :11,
            reserved2   :1;   
    };
    
    struct PT {
        PTE entries[512];
    } __attribute__((packed));
    
    struct PD {
        PDE entries[512];
        PT* tables[512];
    } __attribute__((packed));
    
    struct PDPT {
        PDPTE entries[512];
        PD* tables[512];
    } __attribute__((packed));
    
    struct PML4 {
        PML4E entries[512];
        PDPT* tables[512];
    } __attribute__((packed));
    
    PML4* pages;
    
   // PageDirectory(const PageDirectory& other);
  //  void operator =(const PageDirectory& other);
    
public:
    PageDirectory();
   // PageDirectory(PageDirectory* other);
    //~PageDirectory();
    
    void map(PTE* page, ulong physAddress, bool user, bool rw);
    void switchTo();
    
    PTE* getPage(ulong virtualAddress, bool make);
    void allocFrame(ulong address, bool isUser, bool isWritable);
    void freeFrame(ulong address);
    
    //void faultHandler();
    inline static ulong getPhysAddress(ulong address) { return address & ~0xFFFFFFFFC0000000; }
};

#endif