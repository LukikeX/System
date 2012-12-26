#include "ATADrive.h"

ATADrive::ATADrive(ATAController* controller, bool isSlave, uint blockCount, ushort* data) {
    this->controller = controller;
    this->isSlave = isSlave;
    this->blockCount = blockCount;
    this->data = data;
}

ATADrive::~ATADrive() {
    delete[] data;
}

void ATADrive::CMDCommon(uint startBlock, uint count) {
    controller->writeByte(ATA_PORT_FEATURES_ERROR, 0);
    controller->writeByte(ATA_PORT_SECT_COUNT, count);
    
    controller->writeByte(ATA_PORT_PARTIAL1, startBlock);
    controller->writeByte(ATA_PORT_PARTIAL2, startBlock >> 8);
    controller->writeByte(ATA_PORT_PARTIAL3, startBlock >> 16);
    
    controller->writeByte(ATA_PORT_DRIVE_SELECT, 0xE0 | (isSlave ? 0x10 : 0) | ((startBlock >> 24) & 0x0F));
}

bool ATADrive::readBlocks(ulong start, uint count, char* data) {
    if (start + count >= blockCount)
        return false;
    
    CMDCommon(start, count);
    controller->writeByte(ATA_PORT_COMMAND, ATA_CMD_READ);
    
    while (!(controller->readByte(ATA_PORT_COMMAND) & 0x08));
    
    for (uint i = 0; i < count * 256; i++) {
        ushort tmp = controller->readWord(ATA_PORT_DATA);
        data[i * 2] = (uchar)tmp;
        data[i * 2 + 1] = (uchar)tmp >> 8;
    }
    
    return true;
}

bool ATADrive::writeBlocks(ulong start, uint count, char* data) {
    if (start + count >= blockCount)
        return false;
    
    CMDCommon(start, count);
    controller->writeByte(ATA_PORT_COMMAND, ATA_CMD_WRITE);
    
    while (!(controller->readByte(ATA_PORT_COMMAND) & 0x08));
    for (uint i = 0; i < count * 256; i++)
        controller->writeWord(ATA_PORT_DATA, data[i * 2] | (data[i * 2 + 1] << 8));
    
    return true;
}