#ifndef MEMORY_H
#define	MEMORY_H

#include "Heap.h"
#include "PhysMem.h"
#include "PageDirectory.h"

class Memory {
public:
    Memory();
    
    static void* alloc(ulong size);
    static void* allocInternal(ulong size);
    static void free(void* p);
    
    static ulong kernelHeapFree() { return kernelHeap.free(); }
    static ulong kernelHeapSize() { return kernelHeap.size(); }
    static void* mkXchgSpace(ulong size);
    
    template<typename T>
    static void clear(T* p, uint count = 1) {
        ulong length = count * sizeof(T);
        for (ulong i = 0; i < length; i++)
            ((char *)p)[i] = 0;
    }
    
    template<typename T>
    static void copy(const T* source, T* destination, uint count = 1, uint offset = 0) {
        for (uint i = 0; i < count; i++)
            destination[i + offset] = source[i];
    }
    
    static Heap kernelHeap;
    static ulong placementAddress;
};


inline void* operator new(ulong size)   { return Memory::alloc(size); }
inline void* operator new[](ulong size) { return Memory::alloc(size); }
inline void operator delete(void* p)    { return Memory::free(p); }
inline void operator delete[](void* p)  { return Memory::free(p); }

inline void* operator new(ulong, void* p)   { return p; }
inline void* operator new[](ulong, void* p) { return p; }
inline void operator delete(void*, void*)   { }
inline void operator delete[](void*, void*) { }

#endif