#include "Adv.h"

bool Adv::readBlock(ulong block, char* buff) {
    return dev->readBlocks(block * blockSize(), blockSize(), buff);
}

bool Adv::writeBlock(ulong block, char* buff) {
    return dev->writeBlocks(block * blockSize(), blockSize(), buff);
}

bool Adv::readOff(ulong block, uint count, char* buff) {
    return dev->readBlocks(block * blockSize(), blockSize() * count, buff);
}

bool Adv::writeOff(ulong block, uint count, char* buff) {
    return dev->writeBlocks(block * blockSize(), blockSize() * count, buff);
}