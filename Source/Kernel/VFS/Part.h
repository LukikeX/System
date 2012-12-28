#ifndef PART_H
#define PART_H

#include <Library/Vector.h>
#include "Partition.h"
#include <Devices/BlockDevice.proto.h>

class Part {
private:
    struct MBREntryT {
        unsigned char bootable;             /* 0 = no, 0x80 = bootable */
        unsigned char startHead;            /* Starting head */
        unsigned short startSector: 6;      /* Starting sector */
        unsigned short startCylinder: 10;   /* Starting cylinder */
        unsigned char ID;                   /* System ID */
        unsigned char endHead;              /* Ending head */
        unsigned short endSector: 6;        /* Ending sector */
        unsigned short endCylinder: 10;     /* Ending cylinder */
        unsigned int startLBA;              /* Starting sector (LBA value) */
        unsigned int size;                  /* Total sector number */
    } __attribute__((packed));
    
    static Vector<BlockDeviceProto *> devs;
    static Vector<Partition *> partitions;
    
    static void readPartitionTable(BlockDeviceProto* dev);
    Part();
    
public:
    static void registerDevice(BlockDeviceProto* dev);
    static void unregisterDevice(BlockDeviceProto* dev);
    
    static BlockDeviceProto* findDevice(String cls, uint idx);
    static Partition* findPartition(BlockDeviceProto* dev, uint idx);
};

#endif