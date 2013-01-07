#include "ATAController.h"
#include "ATADrive.h"
#include <DeviceManager/Device.h>
#include <VFS/Part.h>


ATAController::ATAController(uint base, uchar number) {
    this->base = base;
    this->number = number;
    
    drives[0] = 0;
    drives[1] = 0;
    
    identify(false);
    identify(true);
}

void ATAController::detect() {
    ATAController* c[2];
    c[0] = new ATAController(ATA_BUS1_BASE, 0);
    c[1] = new ATAController(ATA_BUS2_BASE, 1);
    Device::registerDevice(c[0]);
    Device::registerDevice(c[1]);
    
    for (uint i = 0; i < 2; i++) {
        for (uint j = 0; j < 2; j++) {
            ATADrive* d = c[i]->drives[j];
            
            if (d) {
                Device::registerDevice(d);
                Part::registerDevice(d);
            }
        }
    }
}

void ATAController::identify(bool slave) {
    if (drives[(slave ? 1 : 0)])
        return;
    
    writeByte(ATA_PORT_DRIVE_SELECT, (slave ? 0xB0 : 0xA0));
    writeByte(ATA_PORT_COMMAND, ATA_CMD_IDENTIFY);
    ushort ret = readByte(ATA_PORT_COMMAND);
    if (!ret)
        return;
    
    while ((ret & 0x88) != 0x08 && (ret & 1) != 1)
        ret = readByte(ATA_PORT_COMMAND);
    
    if ((ret & 1) == 1)
        return;
    
    ushort* data = new ushort[256];
    for (uint i = 0; i < 256; i++)
        data[i] = readWord(ATA_PORT_DATA);
    
    uint blocks = (data[61] << 16) | data[60];
    if (blocks)
        drives[(slave ? 1 : 0)] = new ATADrive(this, slave, blocks, data);
}