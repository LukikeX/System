#ifndef BLOCKDEVICE_PH
#define BLOCKDEVICE_PH

#include <Devices/Device.proto.h>

class BlockDeviceProto: public DeviceProto {
public:
    virtual bool readBlocks(ulong start, uint count, char *data) = 0;
    virtual bool writeBlocks(ulong start, uint count, char *data) = 0;
    virtual ulong blocks() const = 0;
    virtual uint blockSize() const = 0;
};

#endif