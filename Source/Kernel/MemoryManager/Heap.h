#ifndef HEAP_H
#define HEAP_H

#include <Core/Mutex.h>
#include "PageDirectory.h"

#define HEAP_MAGIC    0xDEADC0DE
#define HEAP_MIN_SIZE 0x20000000

class Heap {
private:
    struct headerT {
        uint magic;
        bool isHole;
        ulong size;
    };
    
    struct footerT {
        uint magic;
        headerT* header;
    };
    
    struct indexT {
        headerT** data;
        ulong size;
    } index;
    
    ulong m_free, start, end;
    bool m_usable;
    bool user, rw;
    
    PageDirectory* pageDir;
    Mutex mutex;
    
    void insertIntoIndex(headerT* e);
    uint findIndexEntry(headerT* e);
    void removeFromIndex(uint idx);
    void removeFromIndex(headerT* e);    
    
    void expand(ulong quantity);
    void contract();
    
public:
    Heap();
    ~Heap();
    
    void create(ulong start, ulong size, ulong idxSize, PageDirectory* pageDir, bool user, bool rw);
    void* alloc(ulong size, bool noExpand = false);
    void free(void* ptr);
    
    bool usable() {
        mutex.waitLock();
        bool ret = m_usable;
        mutex.unlock();
        return ret;
    }
    
    ulong size() {
        mutex.waitLock();
        ulong ret = end - start;
        mutex.unlock();
        return ret;
    }
    
    ulong free() {
        mutex.waitLock();
        ulong ret = end - start;
        mutex.unlock();
        return ret;
    }
};

#endif