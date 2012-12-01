#ifndef PS2KEYBOARD_H
#define PS2KEYBOARD_H

#include "Keyboard.proto.h"

class PS2Keyboard : public KeyboardProto {
private:
    bool escaped;
    
public:
    PS2Keyboard();
    String getName() const { return "Standard PS2 keyboard"; };
    String getClass() const { return "keyboard.ps2"; }
    
    void IRQHandler(IDT::regs *);
    void updateLeds(uint status);
};

#endif