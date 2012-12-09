#include "Heap.h"

Heap::Heap(): mutex(MUTEX_FALSE) {
    m_usable = false;
    index.data = 0;
    index.size = 0;
}

Heap::~Heap() {
    //TODO: free pages....
}

void* Heap::alloc(ulong size, bool noExpand) {
    mutex.waitLock();
    
    ulong newSize = size + sizeof(headerT) + sizeof(footerT);
    ulong iter = 0;
    
    while (iter < index.size) {
        if (index.data[iter]->size >= newSize)
            break;
        iter++;
    }
    
    if (iter == index.size) {
        if (noExpand) {
            mutex.unlock();
            //throw new MemoryException...
            return 0;
        }
        
        expand((size & 0xFFFFFFFFFFFFF000) & 0x1000);
        mutex.unlock();
        return alloc(size, true);
    }
    
    headerT* header = index.data[iter];
    footerT* footer = (footerT *)((ulong)header + header->size - sizeof(footerT));
    header->isHole = false;
    
    removeFromIndex(header);
    
    if (header->size > (newSize + sizeof(headerT) + sizeof(footerT))) {
        header->size = newSize;
        
        footerT* newFooter = (footerT *)((ulong)header + newSize - sizeof(footerT));
        newFooter->header = header;
        newFooter->magic = HEAP_MAGIC;
        
        headerT* newHeader = (headerT *)((ulong)header + newSize);
        newHeader->isHole = true;
        newHeader->magic = HEAP_MAGIC;
        newHeader->size = ((ulong)footer - (ulong)newHeader + sizeof(footerT));
        
        footer->header = newHeader;
        footer->magic = HEAP_MAGIC;
        
        insertIntoIndex(newHeader);
    }
    
    m_free -= header->size;
    mutex.unlock();
    
    return (void *)((ulong)header + sizeof(headerT));
}

void Heap::free(void* ptr) {
    if (!ptr)
        return;
    
    headerT* header = (headerT *)((ulong)ptr - sizeof(headerT));
    footerT* footer = (footerT *)((ulong)header + header->size - sizeof(footerT));
    if (header->magic != HEAP_MAGIC || footer->magic != HEAP_MAGIC)
        return;
    
    mutex.waitLock();
    m_free += header->size;
    
    footerT* prevFooter = (footerT *)((ulong)header - sizeof(footerT));
    if (prevFooter->magic == HEAP_MAGIC && prevFooter->header->isHole) {
        header = prevFooter->header;
        removeFromIndex(header);
        
        footer->header = header;
        header->size = ((ulong)footer - (ulong)header + sizeof(footerT));
    }
    
    headerT* nextHeader = (headerT *)((ulong)footer + sizeof(footerT));
    if (nextHeader->magic == HEAP_MAGIC && nextHeader->isHole) {
        removeFromIndex(nextHeader);
        footer = (footerT *)((ulong)footer + nextHeader->size);

        footer->header = header;
        header->size = ((ulong)footer - (ulong)header + sizeof(footerT));
    }
    
    header->isHole = true;
    insertIntoIndex(header);
    
    if ((ulong)footer == (end - sizeof(footerT)) && header->size >= 0x2000 && (end - start > HEAP_MIN_SIZE))
        contract();
    
    mutex.unlock();
}

void Heap::create(ulong start, ulong size, ulong idxSize, PageDirectory* pageDir, bool user, bool rw) {
    if (m_usable)
        return;
    
    if (start & 0x0FFF)
        start = (start & 0xFFFFFFFFFFFFF000) + 0x1000;
    
    if (size & 0x0FFF)
        size = (size & 0xFFFFFFFFFFFFF000) + 0x1000;
    
    this->start = start + idxSize;
    this->end = start + size;
    this->pageDir = pageDir;
    this->user = user;
    this->rw = rw;
    
    for (ulong i = start; i < end; i += 0x1000)
        pageDir->allocFrame(i, user, rw);
    pageDir->switchTo();
    return;
    index.data = (headerT **)start;
    index.size = 0;
    
    headerT* hole = (headerT *)start;
    hole->size = (end - start);
    hole->magic = HEAP_MAGIC;
    hole->isHole = true;
    
    footerT* holeFooter = (footerT *)(end - sizeof(footerT));
    holeFooter->header = hole;
    holeFooter->magic = HEAP_MAGIC;
    
    insertIntoIndex(hole);
    m_usable = true;
    m_free = (end - start);
    mutex.unlock();
}

void Heap::expand(ulong quantity) {
    if (quantity & 0x0FF)
        quantity = (quantity & 0xFFFFFFFFFFFFF000) + 0x1000;
    
    ulong newEnd = end + quantity;
    
    for (ulong i = end; i < newEnd; i += 0x1000)
        pageDir->allocFrame(i, user, rw);
    
    footerT* lastFooter = (footerT *)(end - sizeof(footerT));
    headerT* lastHeader = lastFooter->header;
    if (lastHeader->isHole) {
        removeFromIndex(lastHeader);
        lastHeader->size += quantity;
        
        lastFooter = (footerT *)(newEnd - sizeof(footerT));
        lastFooter->magic = HEAP_MAGIC;
        lastFooter->header = lastHeader;
        
        insertIntoIndex(lastHeader);
    } else {
        lastHeader = (headerT *)end;
        lastFooter = (footerT *)(newEnd - sizeof(footerT));
        
        lastHeader->isHole = true;
        lastHeader->magic = HEAP_MAGIC;
        lastHeader->size = quantity;
        
        lastFooter->magic = HEAP_MAGIC;
        lastFooter->header = lastHeader;
        
        insertIntoIndex(lastHeader);
    }
    
    end = newEnd;
    m_free += quantity;
}

void Heap::contract() {
    footerT* lastFooter = (footerT *)(end - sizeof(footerT));
    headerT* lastHeader = lastFooter->header;
    if (!lastHeader->isHole)
        return;
    
    ulong quantity = 0;
    while ((end - start) - quantity > HEAP_MIN_SIZE && (lastHeader->size - quantity) > 0x1000)
        quantity += 0x1000;
    
    if (!quantity)
        return;
    
    ulong newEnd = end - quantity;
    m_free -= quantity;
    
    removeFromIndex(lastHeader);
    lastHeader->size -= quantity;
    lastFooter = (footerT *)(ulong(lastFooter - quantity));
    lastFooter->magic = HEAP_MAGIC;
    lastFooter->header = lastHeader;
    insertIntoIndex(lastHeader);
    
    for (ulong i = newEnd; i < end; i += 0x1000)
        pageDir->freeFrame(i);
    
    end = newEnd;
}

void Heap::removeFromIndex(uint idx) {
    index.size--;
    idx++;
    while (idx < index.size) {
        index.data[idx - 1] = index.data[idx];
        idx++;
    }
}

void Heap::removeFromIndex(headerT* e) {
    uint i = findIndexEntry(e);
    if (i != (uint)-1)
        removeFromIndex(i);
}

uint Heap::findIndexEntry(headerT* e) {
    for (uint i = 0; i < index.size; i++) {
        if (index.data[i] == e)
            return i;
    }
    return (uint)-1;
}

void Heap::insertIntoIndex(headerT* e) {
    if ((index.size * sizeof(headerT *)) + (ulong)index.data >= start)
        return;
    
    uint iter = 0;
    while (iter < index.size && (ulong)index.data[iter] >= start)
        if (index.data[iter++] == e)
            return;
        
    if (iter == index.size)
        index.data[index.size++] = e;
    else {
        uint pos = iter;
        iter = index.size;
        while (iter > pos) {
            index.data[iter] = index.data[iter - 1];
            iter--;
        }

        index.size++;
        index.data[pos] = e;
    }
}