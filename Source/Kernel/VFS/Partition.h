#ifndef PARTION_H
#define PARTION_H

#include <Devices/BlockDevice.proto.h>
#include "BlockCache.h"

class Partition {
private:
    BlockDeviceProto* dev;
    BlockCache<BlockDeviceProto> cache;
    ulong start, count;
    uchar number;
    
public:
    Partition(BlockDeviceProto* dev, uchar number, ulong start, ulong count);
    
    bool readBlocks(ulong start, uint count, char* data);
    bool writeBlocks(ulong start, uint length, char* data);
    
    inline const BlockDeviceProto* getDevice() const { return dev; }
    inline ulong getStartBlock() const { return start; }
    inline ulong getBlockCount() const { return count; }
    inline uchar getPartNumber() const { return number; }
    inline uint blockSize() const { return dev->blockSize(); }
    //inline void syncCache() { cache.sync(); }
};


#endif