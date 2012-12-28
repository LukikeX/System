#ifndef BLOCKCACHE_H
#define BLOCKCACHE_H

#include <Core/typedef.h>
#include <DeviceManager/Time.h>

template <typename T>
class BlockCache {
private:
    T* dev;
    uint count;
    
    struct cachedBlock {
        ulong id;
        uint lastUse;
        bool dirty;
    } *cacheInfo;
    
    char* cache;
    
    bool getCache(ulong block, char* data) const;
    bool setCache(ulong block, char* data, bool dirty = false);
    
public:
    BlockCache(T* dev);
    ~BlockCache();
    void init(uint count);
    void sync();
    
    bool readBlocks(ulong start, uint count, char* data);
    bool writeBlocks(ulong start, uint count, char* data);
};

#include "BlockDevice.cpp.h"

#endif