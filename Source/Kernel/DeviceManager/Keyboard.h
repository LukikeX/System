#ifndef DM_KEYBOARD_H
#define DM_KEYBOARD_H

#include <Core/typedef.h>
#include <Library/Vector.h>
#include <Devices/Keyboard/Keyboard.defs.h>
#include <VTManager/VirtualTerminal.proto.h>

class Keyboard {
private:
    static const uchar controllKeys[];
    static uint status;
    static VirtualTerminal* focusedVT;
    
    static void process(const keyStatus& ks);
    
public:
    Keyboard();
    static void keyPress(uchar code);
    static void keyRelease(uchar code);
    static void updateLeds();
    static void setFocus(VirtualTerminal* vt);
};

#endif