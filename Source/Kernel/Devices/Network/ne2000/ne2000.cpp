#include "ne2000.h"
#include "DeviceManager/Device.h"
#include <DeviceManager/PCI.h>
#include <Core/IO.h>
#include <NetworkManager/Ethernet.h>


#include <Core/Loader.h>
NE2000::NE2000() {
    dev = 0;
    if (!probePCI())
        return;
    
    getHarwareAddress();
    
    //setup
    IO::outB(ioAddress + NE_RESET, IO::inB(ioAddress + NE_RESET));
    
    uint timeout = 500;
    while (timeout-- && !(IO::inB(ioAddress + INTERRUPTSTATUS) & 0x80));
    if (!timeout)
        return;
    
    IO::outB(ioAddress + INTERRUPTSTATUS, 0xFF);
    
    IO::outB(ioAddress + COMMAND, 0x21);
    IO::outB(ioAddress + DATACONFIGURATION, dcr);
    IO::outB(ioAddress + REMOTEBYTECOUNT0, 0);
    IO::outB(ioAddress + REMOTEBYTECOUNT1, 0);
    IO::outB(ioAddress + RECEIVECONFIGURATION, 0x0E);
    IO::outB(ioAddress + TRANSMITCONFIGURATION, 0x04);
    IO::outB(ioAddress + TRANSMITPAGE, 0x40);
    IO::outB(ioAddress + PAGESTART, 0x46);
    IO::outB(ioAddress + BOUNDARY, 0x46);
    IO::outB(ioAddress + PAGESTOP, 0x60);
    IO::outB(ioAddress + INTERRUPTSTATUS, 0xFF);
    IO::outB(ioAddress + INTERRUPTMASK, 0x1F);
    
    IO::outB(ioAddress + COMMAND, 0x61);
    IO::outB(ioAddress + CURRENT, nextPacket = PSTART + 1);
    
    IO::outB(ioAddress + COMMAND, 0x22);
    IO::outB(ioAddress + TRANSMITCONFIGURATION, 0);
    
    Device::requestIRQ(this, irq + 32);
}

bool NE2000::probePCI() {
    dev = PCI::getDev(0x10EC, 0x8029);
    if (!dev)
        return false;
    
    dev->readBases(7);
    ioAddress = dev->getBase(0) & ~3;
    irq = dev->getIRQ();
    
    return true;
}

void NE2000::getHarwareAddress() {
    IO::outB(ioAddress + REMOTEBYTECOUNT0, 0x20);
    IO::outB(ioAddress + REMOTEBYTECOUNT1, 0x00);
    
    IO::outB(ioAddress + REMOTESTARTADDRESS0, 0x00);
    IO::outB(ioAddress + REMOTESTARTADDRESS1, 0x00);
    
    IO::outB(ioAddress + COMMAND, E8390_START | E8390_NODMA | E8390_PAGE0);
    
    uchar buffer[8];
    int wordlen = 2;
    
    for (uint i = 0; i < 32; i += 2) {
        buffer[i] = IO::inB(ioAddress + IOPORT);
        buffer[i + 1] = IO::inB(ioAddress + IOPORT);
        
        if (buffer[i] != buffer[i + 1])
            wordlen = 1;
    }
    
    if (wordlen == 2) {
        wordMode = 1;
        for (uint i = 0; i < 16; i++)
            saprom[i] = buffer[i * 2];
        
        IO::outB(ioAddress + DATACONFIGURATION, dcr);
        return;
    } else
        wordMode = 0;
}

Packet* NE2000::readPacket() {
    IO::outB(ioAddress + REMOTEBYTECOUNT0, 4);
    IO::outB(ioAddress + REMOTEBYTECOUNT1, 0);
    IO::outB(ioAddress + REMOTESTARTADDRESS0, 0);
    IO::outB(ioAddress + REMOTESTARTADDRESS1, nextPacket);
    IO::outB(ioAddress + COMMAND, E8390_RREAD | E8390_START);
    
    uchar rsr = IO::inB(ioAddress + NE_DATA);
    uchar next = IO::inB(ioAddress + NE_DATA);
    uint len = IO::inB(ioAddress + NE_DATA);
    len += IO::inB(ioAddress + NE_DATA) << 8;
    
    IO::outB(ioAddress + INTERRUPTSTATUS, 0x40);
    
    uchar* data;
    if ((rsr & 31) == 1 && next >= PSTART && next <= PSTOP && len <= 1532) {
        data = new uchar[len];
        IO::outB(ioAddress + REMOTEBYTECOUNT0, len);
        IO::outB(ioAddress + REMOTEBYTECOUNT1, len >> 8);
        IO::outB(ioAddress + REMOTESTARTADDRESS0, 4);
        IO::outB(ioAddress + REMOTESTARTADDRESS1, nextPacket);
        IO::outB(ioAddress + COMMAND, E8390_RREAD | E8390_START);
        
        for (uint i = 0; i < len; i++)
            data[i] = IO::inB(ioAddress + NE_DATA);
        
        IO::outB(ioAddress + INTERRUPTSTATUS, 0x40);
        nextPacket = (next == PSTOP) ? PSTART : next;
    }
    
    if (nextPacket == PSTART)
        IO::outB(ioAddress + BOUNDARY, PSTOP - 1);
    else 
        IO::outB(ioAddress + BOUNDARY, nextPacket - 1);
    
    return new Packet(data, len);
}

int NE2000::writePacket(Ethernet* eth, Packet* packet) {
    
    return 0;
}

void NE2000::IRQHandler(IDT::regs*) {
    
    *kvt << "net irq";
    while (true) {
        uchar status = IO::inB(ioAddress + INTERRUPTSTATUS);
        IO::outB(ioAddress + INTERRUPTSTATUS, status);
        
        if (status & 1) {
            
        } else if (status & 2) {}
        else 
            break;
    }
    
    
    IO::outB(0xA0, 0x20);
    IO::outB(0x20, 0x20);
}