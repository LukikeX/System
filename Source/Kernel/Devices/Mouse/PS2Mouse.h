#ifndef PS2MOUSE_H
#define PS2MOUSE_H

#include "Mouse.proto.h"
#include <Core/IO.h>
#include <DeviceManager/Device.h>
#include <DeviceManager/Display.h>

class PS2Mouse : public MouseProto {
private:
    int x, y;
    uchar no;
    bool left, right;
    
    void wait(char type);
    void write(char str);
    char read();
    
public:
    PS2Mouse();
    state_T getState() const;
    void IRQHandler(IDT::regs*);
    
    String getName() const { return String("Standard PS2 Mouse"); }
    String getClass() const { return String("mouse.ps2"); }
};


#endif