template <typename T>
BlockCache<T>::BlockCache(T* dev) {
    this->dev = dev;
    count = 0;
}

template <typename T>
BlockCache<T>::~BlockCache() {
    sync();
    
    count = 0;
    delete cache;
    delete cacheInfo;
}

template <typename T>
void BlockCache<T>::sync() {
    for (uint i = 0; i < count; i++) {
        if (cacheInfo[i].dirty)
            dev->writeBlocks(cacheInfo[i].id, 1, cache + (i * dev->blockSize()));
    }
}

template <typename T>
void BlockCache<T>::init(unsigned int count) {
    this->count = count;
    cacheInfo = new cachedBlock[count];
    
    for (uint i = 0; i < count; i++) {
        cacheInfo[i].id = 0;
        cacheInfo[i].lastUse = 0;
        cacheInfo[i].dirty = false;
    }
    
    cache = new char[count * dev->blockSize()];
}

template <typename T>
bool BlockCache<T>::getCache(ulong block, char* data) const {
    for (uint i = 0; i < count; i++) {
        if (cacheInfo[i].id == block && cacheInfo[i].lastUse) {
            cacheInfo[i].lastUse = Time::time();
            Memory::copy(cache + (i * dev->blockSize()), data, dev->blockSize());
            return true;
        }
    }
    return false;
}

template <typename T>
bool BlockCache<T>::setCache(ulong block, char* data, bool dirty) {
    uint best = 0;
    
    for (uint i = 0; i < count; i++) {
        if (cacheInfo[i].id == block) {
            best = i;
            break;
        }
        
        if (cacheInfo[i].lastUse < cacheInfo[best].lastUse)
            best = i;
    }
    
    if (best > count)
        return false;
    
    if (cacheInfo[best].dirty && (cacheInfo[best].id != block || !dirty))
        dev->writeBlocks(cacheInfo[best].id, 1, cache + (best * dev->blockSize()));
    
    cacheInfo[best].id = block;
    cacheInfo[best].lastUse = Time::time();
    cacheInfo[best].dirty = dirty;
    Memory::copy(data, cache + (best * dev->blockSize()), dev->blockSize);
    return true;
}

template <typename T>
bool BlockCache<T>::readBlocks(ulong start, uint count, char* data) {
    if (count == 1) {
        if (getCache(start, data))
            return true;
        
        if (!dev->readBlocks(start, count, data))
            return false;
        
        setCache(start, data);
        return true;
    } else
        return dev->readBlocks(start, count, data);
}

template <typename T>
bool BlockCache<T>::writeBlocks(ulong start, uint count, char* data) {
    if (count == 1) {
        if (!setCache(start, data, true))
            return dev->writeblocks(start, count, data);
        return true;
    } else
        return dev->writeBlocks(start, count, data);
}