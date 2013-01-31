#ifndef ADV_INODE_H
#define ADV_INODE_H

class AdvInode {
private:
    ushort mode;
    ushort uid;
    uint size;
    uint accessTime;
    uint changeTime;
    uint modificationTime;
    uint deletionTime;
    ushort gid;
    ushort linkCount;
    uint sectorCount;
    uint flags;
    uint osl;
    uint blocks[15];
    uint genNum;
    uint fileAcl;
    uint sizeUp;
    uchar pad[20];
    uint number;
    
    Adv* parent;
    
    int getBlock(uint inode, ulong* offset);
    bool read(uint inode);
    
} __attribute__((packed));

#endif