#include "Packet.h"
#include "MemoryManager/Memory.h"

Packet::Packet(uchar* data, ushort len) {
    this->data = new uchar[len];
    this->len = len;
    
    Memory::copy(data, this->data, len);
}

Packet::Packet(const Packet& other) {
    data = new uchar[other.len];
    len = other.len;
    
    Memory::copy(other.data, data, len);
}

Packet::~Packet() {
    delete[] data;
}

ushort Packet::checksum() {
    uint sum = 0;
    ushort* p = (ushort *)data;
    ushort len = this->len;
    
    while (len > 1) {
        sum += *p++;
        if (sum < 0xFFFF)
            sum = (sum & 0xFFFF) + 1;
        len -= 2;
        
        if (len == 1) {
            sum += *(uchar *)p;
            if (sum > 0xFFFF)
                sum = (sum & 0xFFFF) + 1;
        }
    }
    
    return (ushort)~sum;
}

void Packet::addHeader(uint size) {
    uchar* newData = new uchar[len + size];
    
    Memory::copy(data, newData, len, size);
    delete[] data;
    len += size;
}