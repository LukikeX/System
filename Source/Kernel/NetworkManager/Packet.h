#ifndef PACKET_H
#define PACKET_H

#include <Core/typedef.h>

class Packet {
private:
    ushort len;
    uchar* data;
    
public:
    Packet(uchar* data, ushort len);
    Packet(const Packet& other);
    ~Packet();
    
    ushort checksum();
    void addHeader(ushort size, uchar* data);
    void delHeader(ushort size);
};

#endif