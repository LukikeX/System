#include "Bitset.h"
#include <MemoryManager/Memory.h>

Bitset::Bitset(uint size, uint* ptr) {
    init(size, ptr);
}

Bitset::Bitset(uint size) {
    init(size, (uint *)Memory::alloc(INDEX_FROM_BIT(size)));
}

void Bitset::init(uint size, uint* ptr) {
    this->size = size;
    data = ptr;
    
    for (uint i = 0; i < INDEX_FROM_BIT(size); i++)
        data[i] = 0;
    
    usedCount = 0;
}

void Bitset::setBit(uint number) {
    data[INDEX_FROM_BIT(number)] |= (1 << OFFSET_FROM_BIT(number));
    usedCount++;
}

void Bitset::clearBit(uint number) {
    data[INDEX_FROM_BIT(number)] &= ~(1 << OFFSET_FROM_BIT(number));
    usedCount--;
}

bool Bitset::testBit(uint number) {
    return data[INDEX_FROM_BIT(number)] & (1 << OFFSET_FROM_BIT(number));
}

uint Bitset::firstFreeBit() {
    for (uint i = 0; i < INDEX_FROM_BIT(size); i++) {
        if (data[i] != 0xFFFFFFFF) {
            for (int j = 0; j < 32; j++) {
                if (!(data[i] & (1 << j)))
                    return i * 32 + j;
            }
        }
    }
    return (uint)-1;
}