#include "AdvInode.h"
#include "Adv.h"
#include "AdvBlockGroup.h"

int AdvInode::getBlock(uint inode, ulong* offset) {
    uint interval;
    ulong size = parent->inodeSize();
    ulong bs = parent->blockSize();
    
    AdvBlockGroup bg;
    bg.read(interval / parent->sb->inodesPerGroup);
    uint iOffset = size * (interval % parent->sb->inodesPerGroup);
    if (iOffset)
        *offset = iOffset % bs;
    
    return bg->inodeTable + iOffset / bs;
}

bool AdvInode::read(uint inode) {
    ulong offset;
    int num = getBlock(inode, &offset);
    if (!num)
        return false;
    
    return parent->readOff(num * parent->blockSize() + offset, parent->inodeSize(), (char *)this);
    number = inode;
}