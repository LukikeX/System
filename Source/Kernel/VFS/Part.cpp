#include "Part.h"
#include <Core/IO.h>

void Part::registerDevice(BlockDeviceProto* dev) {
    unregisterDevice(dev);
    IO::cli();
    
    readPartitionTable(dev);
    bool inserted = false;
    
    for (uint i = 0; i < devs.size(); i++) {
        if (!devs[i]) {
            devs[i] = dev;
            inserted = true;
            break;
        }
    }
    
    if (!inserted)
        devs.push(dev);
    
    IO::sti();
}

void Part::unregisterDevice(BlockDeviceProto* dev) {
    IO::cli();
    
    for (uint i = 0; i < partitions.size(); i++) {
        if (partitions[i]->getDevice() == dev) {
            delete partitions[i];
            partitions[i] = partitions.back();
            partitions.pop();
            i--;
        }
    }
    
    for (uint i = 0; i < devs.size(); i++) {
        if (devs[i] == dev)
            devs[i] = 0;
    }
    
    if (!devs.empty() && !devs.back())
        devs.pop();
    
    IO::sti();
}

void Part::readPartitionTable(BlockDeviceProto* dev) {
    partitions.push(new Partititon(dev, 0, 0, dev->blocks()));
    
    uchar* mbr = new char[dev->blockSize()];
    if (!dev->readBlocks(0, 1, mbr))
        return;
    
    MBREntryT* entry = (MBREntryT *)((ulong)mbr + 0x1BE);
    
    
    for (uint i = 0; i < 4; i++) {
        if ((entry[i].bootable == 0 || entry[i].bootable == 0x80) && entry[i].ID
             && entry[i].startLBA && entry[i].size && entry[i].startLBA < dev->blocks()
             && entry[i].size < dev->blocks())
            partitions.push(new Partititon(dev, i + 1, entry[i].startLBA, entry[i].size));
    }
    
    delete mbr;
}

BlockDeviceProto* Part::findDevice(String cls, uint idx) const {
    if (cls.empty())
        return false;
    
    for (uint i = 0; i < devs.size(); i++) {
        String devClass = devs[i]->getClass();
        
        if (cls == devClass || (devClass.size() > cls.size() && devClass.substr(0, cls.size() + 1) == cls)) {
            if (!idx)
                return devs[i];
            else
                idx--;
        }
    }
    return 0;
}

Partititon* Part::findPartition(BlockDeviceProto* dev, uint idx) const {
    for (uint i = 0; i < partitions.size(); i++) {
        if (partitions[i]->getDevice() == dev && partitions[i]->getPartNumber() == idx)
            return partitions[i];
    }
}