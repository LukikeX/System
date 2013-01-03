#include "Ext2.h"

void Ext2::inodeToBlock(int inode, int* block, int* offset) {
    int group, groupBlock;
    
    inode--;
    if (inode < 0)
        return;
    
    group = inode / fs->inodesPerGrp;
    groupBlock = fs->groupDesc[group].inodeTable * fs->blksize / 512;
    *block = groupBlock + (((inode - group * fs->inodesPerGrp) * fs->inodesize) / 512);
    *offset = inode * fs->inodesize;
    if (*offset > 512)
        *offset = *offset % 512;
}

Ext2::inode_T* Ext2::loadInode(int inodeNum) {
    inode_T* inode, *inodep;
    int block, offset;
    char* buffer = new char[fs->blksize];
    
    inodeToBlock(inodeNum, &block, &offset);
    part->readBlocks(block, fs->blksize / 512, buffer);
    inode = new inode_T;
    inodep = (inode_T *)buffer;
    Memory::copy(buffer, (char *)inode, sizeof(inode_T), offset);
    
    delete buffer;
    return inode;
}

int Ext2::searchDir(inode_T* ino, char* name) {
    char* buffer;
    dirEntry_T* dir;
    uint count, bufSize;
    int inode = -1;
    char* filename;
    
    if (!ino || ino->mode != EXT2_FT_DIR)
        return -1;
    
    bufSize = ino->blocks * 512;
    buffer = new char[bufSize];
    part->readBlocks(ino->block[0] * fs->blksize / 512, ino->blocks, buffer);
    dir = (dirEntry_T *)buffer;
    
    count = 0;
    while (count + 8 < bufSize) {
        if (!dir->inode) {
            count += dir->reclen;
            dir = (dirEntry_T *)((char *)dir + dir->reclen);
            continue;
        }
        
        filename = new char[dir->namelen + 1];
        Memory::copy(dir->name, filename, dir->namelen);
        filename[dir->namelen] = 0;
        
        if (String(name) == String(filename)) {
            inode = dir->inode;
            delete filename;
            delete buffer;
            break;
        }
        delete filename;
        count += dir->reclen;
        dir = (dirEntry_T *)((char *)dir + dir->reclen);
    }
    
    delete buffer;
    return inode;
}

