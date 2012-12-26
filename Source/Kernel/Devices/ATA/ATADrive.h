#ifndef ATADRIVE_H
#define ATADRIVE_H

#include <Devices/BlockDevice.proto.h>
#include "ATAController.h"

class ATADrive : public BlockDeviceProto {
    friend class ATAController;
private:
    ATAController* controller;
    bool isSlave;
    uint blockCount;
    ushort* data;
    
    void CMDCommon(uint startBlock, uint count);
    
public:
    ATADrive(ATAController* controller, bool isSlave, uint blockCount, ushort* data);
    ~ATADrive();
    
    bool readBlocks(ulong start, uint count, char* data);
    bool writeBlocks(ulong start, uint count, char* data);
    ulong blocks() const { return blockCount; }
    uint blockSize() const { return 512; }
    
    String getName() const { return String("ATA Drive ") + String::number((uint)controller->getNumber())
                                    + (isSlave ? String(" Slave ") : String(" Master ")) 
                                    + String::number(blockCount / (1024 * 1024 / blockSize())) + String(" MB"); }
    String getClass() const { return "block.ata"; }
};

#endif