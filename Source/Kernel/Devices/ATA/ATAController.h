#ifndef ATACONTROLLER_H
#define ATACONTROLLER_H

#include <Devices/Device.proto.h>
#include <Core/IO.h>

#define ATA_BUS1_BASE 0x1F0
#define ATA_BUS2_BASE 0x170

#define ATA_PORT_DATA           0
#define ATA_PORT_FEATURES_ERROR 1
#define ATA_PORT_SECT_COUNT     2
#define ATA_PORT_PARTIAL1       3
#define ATA_PORT_PARTIAL2       4
#define ATA_PORT_PARTIAL3       5
#define ATA_PORT_DRIVE_SELECT   6
#define ATA_PORT_COMMAND        7

#define ATA_CMD_IDENTIFY 0xEC
#define ATA_CMD_READ     0x20
#define ATA_CMD_WRITE    0x30

class ATADrive;

class ATAController : public DeviceProto {
    friend class ATADrive;
private:
    uint base;
    uchar number;
    ATADrive* drives[2];
    
    ATAController(uint base, uchar number);
    void identify(bool slave);
    
    void writeByte(ushort port, uchar data) { IO::outB(base + port, data); }
    void writeWord(ushort port, ushort data) { IO::outW(base + port, data); }
    uchar readByte(ushort port) { return IO::inB(base + port); }
    ushort readWord(ushort port) { return IO::inW(base + port); }
    
public:
    static void detect();
    
    String getName() const { return String("ATA Controler #") + String::number(number); }
    String getClass() const { return "controller.ata"; }
    inline uchar getNumber() const { return number; }
};


#endif