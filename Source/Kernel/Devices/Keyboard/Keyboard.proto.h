#ifndef KEYBOARD_PH
#define KEYBOARD_PH

#include <Core/typedef.h>
#include <Devices/Device.proto.h>

class KeyboardProto : public DeviceProto {
public:
    virtual ~KeyboardProto() { }
    virtual void updateLeds(uint status) = 0;
};

#endif