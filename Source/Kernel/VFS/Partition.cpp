#include "Partition.h"
#include <DeviceManager/Device.h>

Partititon::Partition(BlockDeviceProto* dev, uchar number, ulong start, ulong count) {
    this->dev = dev;
    this->number = number;
    this->start = start;
    this->count = count;
    Device::registerDevice(this);
    
    cache.init(10 + (dev->blocks() / 1000 > 100 ? 100 : dev->blocks() / 1000));
}

bool Partititon::readBlocks(ulong start, uint count, char* data) {
    return cache.readBlocks(start + this->start, count, data);
}

bool Partititon::writeBlocks(ulong start, uint length, char* data) {
    int count = length / dev->blockSize() + 1;
    int len = count * dev->blockSize();
    
    char* buffer = new char[len];
    Memory::clear(buffer, len);
    Memory::copy(data, buffer, len);
    
    if (start + count > this->start + this->count)
        return false;
    
    bool ret = cache.writeBlocks(start + this->start, count, buffer);
    delete[] buffer;
    return ret;
}