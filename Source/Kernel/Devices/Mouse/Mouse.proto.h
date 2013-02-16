#ifndef MOUSE_PH
#define MOUSE_PH

#include <Core/typedef.h>
#include <Devices/Device.proto.h>

class MouseProto: public DeviceProto {
public:
    struct state_T {
        int x, y;
        bool left, right;
    };
    
    virtual state_T getState() const = 0;
};

#endif