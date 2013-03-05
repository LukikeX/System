#ifndef NETWORK_NE2000_H
#define NETWORK_NE2000_H

#include <Devices/Device.proto.h>
#include <Devices/PCI/PCIDev.h>
#include <NetworkManager/Packet.h>
#include <NetworkManager/Ethernet.h>

#define NE_RESET 0x1F
#define NE_DATA  0x10

class NE2000 : public DeviceProto {
private:
    PCIDev* dev;
    ushort ioAddress;
    uchar saprom[16];
    void* recvPacketBuffer;
    uint packetLength;
    ushort nextPacket;
    uint irq;
    uint wordMode;
    
public:
    NE2000();
    bool probePCI();
    void getHarwareAddress();
    Packet* readPacket();
    int writePacket(Ethernet* eth, Packet* packet);
    void IRQHandler(IDT::regs*);
    
    String getName() const { return "Standard NE2000 network driver"; }
    String getClass() const { return "network.ne2000"; }
};

#endif