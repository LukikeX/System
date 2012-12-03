#include "PS2Keyboard.h"
#include <DeviceManager/Device.h>
#include <Core/IO.h>
#include <DeviceManager/Keyboard.h>

#include <Core/Loader.h>

PS2Keyboard::PS2Keyboard() {
    Device::requestIRQ(this, 1);
    
    uchar tmp = IO::inB(0x60), tmp2 = 0;
    while (tmp != tmp2) {
        tmp2 = tmp;
        tmp = IO::inB(0x60);
    }

    escaped = false;
}

void PS2Keyboard::IRQHandler(IDT::regs*) {
    uchar code = IO::inB(0x60);
    if (code == 0xE0)
        escaped = true;
    else {
        if (code & 0x80) {
            if (escaped)
                Keyboard::keyRelease(code);
            else
                Keyboard::keyRelease(code & 0x7F);
        } else {
            if (escaped)
                Keyboard::keyPress(code | 0x80);
            else
                Keyboard::keyPress(code);
        }
    }
}

void PS2Keyboard::updateLeds(uint status) {
    uchar tmp = 0;
    if (status & STATUS_SCRL)
        tmp |= 1;
    if (status & STATUS_NUM)
        tmp |= 2;
    if (status & STATUS_CAPS)
        tmp |= 4;
    
    IO::outB(0x60, tmp);
}