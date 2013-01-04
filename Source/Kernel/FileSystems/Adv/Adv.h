#ifndef ADV_H
#define ADV_H

#include <VFS/FileSystem.proto.h>

#define ADV_MAGIC    0xB3F4
#define ADV_NAME_LEN 255

class Adv : public FileSystem {
private:
    uint blockSize;
    uint inodeSize;
    uint blockGroups;
    uint blockPerGroup;
    uint inodesPerGroup;
};

#endif