#ifndef ADV_BLOCKGROUP_H
#define ADV_BLOCKGROUP_H

#include "Adv.h"

class AdvBlockGroup {
    friend class AdvInode;
    uint blockBitmap;
    uint inodeBitmap;
    uint inodeTable;
    ushort freeBlocks;
    ushort freeInodes;
    ushort usedDirectories;
    ushort padding;
    uint reserved[3];
    
    Adv* parent;
    
    int getBlock(int group);
    bool read(int group);
    int update(int group);
    
public:
    
    
} __attribute__((packed));

#endif