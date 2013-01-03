#ifndef EXT2_H
#define EXT2_H

#include <VFS/FileSystem.proto.h>

#define EXT2_MAGIC       0xEF53
#define EXT2_NAME_LEN    255

#define EXT2_FT_UNKNOWN  0
#define EXT2_FT_REG_FILE 1
#define EXT2_FT_DIR      2
#define EXT2_FT_CHRDEV   3
#define EXT2_FT_BLKDEV   4
#define EXT2_FT_FIFO     5
#define EXT2_FT_SOCK     6
#define EXT2_FT_SYMLINK  7
#define EXT2_FT_MAX      8

class Ext2 : public FileSystem {
private:
    struct superBlock_T {
        uint32_t inodesCount;		/* Inodes count */
        uint32_t blocksCount;		/* Blocks count */
        uint32_t rBlocksCount;		/* Reserved Blocks Count */
        uint32_t freeBlocksCount;	/* Free Blocks count */
        uint32_t freeInodesCount;	/* Free Inodes count */
        uint32_t firstDataBlock;	/* Usually 1 if blks == 1024, else 0 */
        uint32_t logBlockSize;		/* 1024 << logBlockSize == blksz in bytes */
        uint32_t logFragSize;		/* Fragment Size */
        uint32_t blocksPerGroup;	/* group blocks */
        uint32_t fragsPerGroup;		/* fragments per group */
        uint32_t inodesPerGroup;	/* groups inodes */
        uint32_t mtime;				/* Time of last mount */
        uint32_t wtime;				/* time of last write */
        uint16_t mntCount;			/* Mount count */
        uint16_t maxMntCount;	 	/* Max Mount Count */
        uint16_t magic;				/* magic signature */
        uint16_t state;				/* file system state */
        uint16_t errors;			/* behaviour when detecting errors */
        uint16_t minorRevLevel;		/* Minor revision */
        uint32_t lastCheck;			/* time of last check */
        uint32_t checkInterval;		/* max time between checks */
        uint32_t createrOS;			/* OS */
        uint32_t revLevel;			/* revision level */
        uint16_t defResUid;			/* default uid for reserved blocks */
        uint16_t defResGid;			/* ditto for gid */
        /* from this point on values are only valid if revLevel > 0, otherwise
        * defaults assumed */
        uint32_t firstIno;			/* default: 11 */
        uint16_t inodeSize;			/* default: 128 */
        uint16_t blockGroupNum;		/* block group num this super is in */
        uint32_t featureCompat;
        uint32_t featureIncompat;
        uint32_t featureROCompat;
        uint8_t  uuid[16];			/* 128bit volume ID */
        uint8_t  volumeName[16];
        uint8_t  lastMounted[64];	/* path last mounted at */
        uint32_t algoBitmap;
        /* performance hints */
        uint8_t  preallocBlocks;
        uint8_t  preallocDirBlocks;
        uint16_t alignment;
        // ext3 stuff
        uint8_t  journalUuid[16];
        uint32_t journalInum;
        uint32_t journalDev;
        uint32_t lastOrphan;
        uint8_t  padding[788];
    };
    
    struct groupDescriptor_T {
        uint32_t blockBitmap;
        uint32_t inodeBitmap;
        uint32_t inodeTable;
        uint16_t freeBlocksCount;
        uint16_t freeInodesCount;
        uint16_t usedDirsCount;
        uint16_t pad;
        uint32_t reserved[3];
    };
    
    struct inode_T {
        uint16_t mode;
        uint16_t uid;
        uint32_t size;
        uint32_t atime;
        uint32_t ctime;
        uint32_t mtime;
        uint32_t dtime;
        uint16_t gid;
        uint16_t linksCount;
        uint32_t blocks;
        uint32_t flags;
        uint32_t osd1;
        uint32_t block[12];
        uint32_t iblock;	/* indirect block */
        uint32_t diblock;	/* double indirect block */
        uint32_t tiblock;	/* tripple indirect block */
        uint32_t generation;
        uint32_t fileAcl;
        uint32_t dirAcl;
        uint32_t faddr;
        uint32_t osd2[3];
    };
    
    struct dirEntry_T {
        uint32_t inode;		/* inode number */
        uint16_t reclen;	/* record length */
        uint8_t  namelen;	/* name length */
        uint8_t  type;		/* file type, see defines above */
        uint8_t  name[EXT2_NAME_LEN];
    };
    
    struct fs_T {
        uint32_t blksize;
        uint32_t inodesize;
        uint32_t blockGroups;
        uint32_t blkPerGrp;
        uint32_t inodesPerGrp;
        groupDescriptor_T *groupDesc;
    };
    
    fs_T* fs;
    Partition* part;
    
    void inodeToBlock(int inode, int* block, int* offset);
    inode_T* loadInode(int inodeNum);
    int searchDir(inode_T* ino, char* name);
    
public:
    
};

#endif