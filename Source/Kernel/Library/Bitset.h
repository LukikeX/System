#ifndef BITSET_H
#define BITSET_H

#include <Core/typedef.h>

#define INDEX_FROM_BIT(a)  (a / 32)
#define OFFSET_FROM_BIT(a) (a % 32)

class Bitset {
private:
    uint size;
    uint* data;
    uint usedCount;
    
public:
    Bitset() {}
    Bitset(uint size);
    Bitset(uint size, uint* ptr);
    ~Bitset() { if (data) delete[] data; }
    
    void init(uint size, uint* ptr);
    
    void setBit(uint number);
    void clearBit(uint number);
    bool testBit(uint number);
    uint firstFreeBit();
    
    inline uint usedBits() { return usedCount; }
};

#endif