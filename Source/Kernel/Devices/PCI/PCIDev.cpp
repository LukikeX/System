#include "PCIDev.h"
#include "Core/IO.h"
#include <DeviceManager/Device.h>


PCIDev::PCIDev(uint bus, uint dev, uint func, common_T* cmn) {
    this->bus = bus;
    this->dev = dev;
    this->func = func;
    common = cmn;
    
    
    irq = readB(PCI_IRQ_PIN);
    if (irq)
        irq = readB(PCI_IRQ_LINE);
    
    Device::registerDevice(this);
}

uint PCIDev::read(int reg, int ts) {
    union {
        confadd_T c;
        uint n;
    } u;
    
    u.n = 0;
    u.c.enable = 1;
    u.c.rsvd = 0;
    u.c.bus = bus;
    u.c.dev = dev;
    u.c.func = func;
    u.c.reg = reg & 0xFC;
    
    IO::outL(PCI_DATA_REG, u.n);
    ushort base = PCI_BASE_REG + (reg & 0x03);
    
    switch (ts) {
        case 1:
            return IO::inB(base);
        case 2:
            return IO::inW(base);
        case 4:
            return IO::inL(base);
        default:
            return 0;
    }
}

void PCIDev::write(int reg, uint value, int ts) {
    union {
        confadd_T c;
        uint n;
    } u;
    
    u.n = 0;
    u.c.enable = 1;
    u.c.rsvd = 0;
    u.c.bus = bus;
    u.c.dev = dev;
    u.c.func = func;
    u.c.reg = reg & 0xFC;
    
    IO::outL(PCI_DATA_REG, u.n);
    ushort base = PCI_BASE_REG + (reg & 0x03);
    
    switch (ts) {
        case 1:
            IO::outB(base, (uchar)value);
            break;
        case 2:
            IO::outW(base, (ushort)value);
            break;
        case 4:
            IO::outL(base, value);
            break;
    }
}

PCIDev::barType PCIDev::getBarType(int barNum) {
    uint temp = readL(0x10 + (barNum << 2));
    
    if (temp & 1) 
        return BAR_TYPE_IO;
    return BAR_TYPE_MEM;
}

uint PCIDev::getBar(int barNum) {
    uint temp = readL(0x10 + (barNum << 2));
    
    if (temp & 1)
        return temp & (~0x03);
    return temp & (~0x0F);
}

void PCIDev::readBases(uint count) {
    Memory::clear(base, 6);
    Memory::clear(size, 6);
    Memory::clear(type, 6);
    
    for (uint i = 0; i < count; i++) {
        uint reg = PCI_BASE_ADDRESS_0 + (i << 2);
        uint l = readB(reg);
        writeB(reg, ~0);
        
        uint sz = readB(reg);
        writeB(reg, l);
        
        if (!sz || sz == 0xFFFFFFFF)
            continue;
        
        if (l == 0xFFFFFFFF)
            l = 0;
        
        if ((l & PCI_BASE_ADDRESS_SPACE) == PCI_BASE_ADDRESS_SPACE_MEMORY) {
            base[i] = l & PCI_BASE_ADDRESS_MEM_MASK;
            size[i] = getSize(sz, PCI_BASE_ADDRESS_MEM_MASK);
            type[i] = PCI_IO_RESOURCE_MEM;
        } else {
            base[i] = l & PCI_BASE_ADDRESS_IO_MASK;
            size[i] = getSize(sz, PCI_BASE_ADDRESS_IO_MASK);
            type[i] = PCI_IO_RESOURCE_IO;
        }
    }
}

uint PCIDev::getSize(uint base, uint mask) {
    uint ret = base & mask;
    ret = ret & ~(ret - 1);
    return ret - 1;
}