#ifndef ADV_H
#define ADV_H

#include <VFS/FileSystem.proto.h>
#include <VFS/Partition.h>

#include "AdvInode.h"

#define ADV_MAGIC    0xB3F4
#define ADV_NAME_LEN 255

class Adv : public FileSystem {
    friend class AdvBlockGroup;
    friend class AdvInode;
private:
    struct superBlock_T {
        uint blockCount;
        uint inodeCount;
        uint reservedBlocks;
        uint freeBlocks;
        uint freeInodes;
        uint firstDataBlock;
        uint blockSize;
        int fragmentSize;

        uint blockPerGroup;
        uint fragmentsPerGroup;
        uint inodesPerGroup;

        uint mountTimer;
        uint writeTime;

        ushort mountCount;
        ushort maxMountCount;
        ushort magic;
        ushort state;
        ushort errors;
        ushort minorRevision;

        uint lastCheckTime;
        uint checkInterval;
        uint creatorOS;
        uint revision;

        ushort defaultResUID;
        ushort defaultResGID;
        uint firstInode;
        ushort inodeSize;
        ushort blockGroupNum;
        uint featuresOPT;
        uint featuresREQ;
        uint featuresRO;

        uint volumeID[4];
        char volumeName[16];
        char lastMounted[64];
        uint algoBitmap;

        uint reserved[205];
    } __attribute__((packed));
    
    superBlock_T* sb;
    int flag;
    Partition* dev;
    char node[16];
    uint blockPrevAlloc;
    AdvInode* root;
    bool rw;
    Mutex* m_node, *m_block, *m_inode, ac_lock;
    
    
    
    inline uint blockSize() { return 1024 << sb->blockSize; }
    inline uint bgCount() { return (sb->blockCount - sb->firstDataBlock + sb->blockPerGroup - 1) / sb->blockPerGroup; }
    inline uint inodeSize() { return sb->inodeSize; }
    
    bool readBlock(ulong block, char* buff);
    bool writeBlock(ulong block, char* buff);
    bool readOff(ulong block, uint count, char* buff);
    bool writeOff(ulong block, uint count, char* buff);
};

#endif