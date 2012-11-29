#include "Keyboard.h"
#include "Device.h"
#include <Devices/Device.proto.h>
#include <Resources/Keymaps/en.kmap>
#include <Devices/Keyboard/Keyboard.proto.h>

uint Keyboard::status;

const uchar Keyboard::controllKeys[] = {
/* 0x00 */	0, KBDC_ESCAPE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KBDC_BACKSPACE, KBDC_TAB,
/* 0x10 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KBDC_ENTER, KBDC_LEFTCTRL, 0, 0,
/* 0x20 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KBDC_LEFTSHIFT, 0, 0, 0, 0, 0,
/* 0x30 */	0, 0, 0, 0, 0, 0, KBDC_RIGHTSHIFT, 0, KBDC_ALT, 0, KBDC_CAPSLOCK, KBDC_F1, KBDC_F2, KBDC_F3, KBDC_F4, KBDC_F5,
/* 0x40 */	KBDC_F6, KBDC_F7, KBDC_F8, KBDC_F9, KBDC_F10, KBDC_NUMLOCK, KBDC_SCRLLOCK, KBDC_KPHOME, KBDC_KPUP, KBDC_KPPGUP, 0, KBDC_KPLEFT, KBDC_KP5, KBDC_KPRIGHT, 0, KBDC_KPEND,
/* 0x50 */	KBDC_KPDOWN, KBDC_KPPGDOWN, KBDC_KPINSERT, KBDC_KPDEL, KBDC_SYSREQ, 0, 0, KBDC_F11, KBDC_F12, 0, 0, 0, 0, 0, 0, 0,
/* 0x60 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 0x70 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 0x80 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 0x90 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KBDC_ENTER, KBDC_RIGHTCTRL, 0, 0,
/* 0xA0 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 0xB0 */	0, 0, 0, 0, 0, KBDC_KPSLASH, 0, KBDC_PRTSCN, KBDC_ALTGR, 0, 0, 0, 0, 0, 0, 0,
/* 0xC0 */	0, 0, 0, 0, 0, 0, 0, KBDC_HOME, KBDC_UP, KBDC_PGUP, 0, KBDC_LEFT, 0, KBDC_RIGHT, 0, KBDC_END,
/* 0xD0 */	KBDC_DOWN, KBDC_PGDOWN, KBDC_INSERT, KBDC_DEL, 0, 0, 0, 0, 0, 0, 0, KBDC_LEFTSUP, KBDC_RIGHTSUP, KBDC_MENU, 0, 0,
/* 0xE0 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 0xF0 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
};

Keyboard::Keyboard() {
    status = 0;
}

void Keyboard::updateLeds() {
    Vector<DeviceProto *> kbd = Device::findDevices("keyboard");
    for (uint i = 0; i < kbd.size(); i++)
        ((KeyboardProto *)(kbd[i]))->updateLeds(status);
}

void Keyboard::keyPress(uchar code) {
    keyStatus ks;
    ks.pressed = true;
    ks.modifiers = status & 0x0F;
    
    uchar cmd = controllKeys[code];
    code &= 0x7F;
    
    if (!cmd)
        cmd = controllKeys[code];
    
    if (!cmd) {
        ks.hasChar = true;
        ks.character = 0;
        
        if ((status & STATUS_ALT) || (status & STATUS_CTRL))
            ks.hasCmd = true;
        
        if ((status & STATUS_SHIFT) ^ (status & STATUS_CAPS)) {
            if (status & STATUS_ALTGR)
                ks.character = keymapNormal[code]; //keymapShiftAltgr
            else {
                if (status & STATUS_CAPS)
                    ks.character = keymapCaps[code];
                else
                    ks.character = keymapShift[code];
            }
        } else {
            if (status & STATUS_ALTGR)
                ks.character = keymapNormal[code]; //keymapAltGr
            else 
                ks.character = keymapNormal[code];
        }
    } else if (cmd >= KBDC_KPINSERT && cmd <= KBDC_KPDEL && (status & STATUS_NUM)) {
        ks.hasChar = true;
        if ((status & STATUS_ALT) || (status & STATUS_CTRL))
            ks.hasCmd = true;
        
        if (cmd == KBDC_KPDEL)
            ks.character = (uint)'.';
        else
            ks.character = (uint)'0' + (cmd - KBDC_KPINSERT);
    } else if (cmd == KBDC_KPSLASH) {
        ks.hasChar = true;
        if ((status & STATUS_ALT) || (status & STATUS_CTRL))
            ks.hasCmd = true;
        ks.hasCmd = true;
    } else if (cmd == KBDC_ALT)
        status |= STATUS_ALT;
    else if (cmd == KBDC_ALTGR)
        status |= STATUS_ALTGR;
    else if (cmd == KBDC_LEFTCTRL || cmd == KBDC_RIGHTCTRL)
        status |= STATUS_CTRL;
    else if (cmd == KBDC_LEFTSHIFT || cmd == KBDC_RIGHTSHIFT)
        status |= STATUS_SHIFT;
    else if (cmd == KBDC_CAPSLOCK) {
        status ^= STATUS_CAPS;
        updateLeds();
    } else if (cmd == KBDC_NUMLOCK) {
        status ^= STATUS_NUM;
        updateLeds();
    } else if (cmd == KBDC_SCRLLOCK) {
        status ^= STATUS_SCRL;
        updateLeds();
    }
     
    if (!ks.hasChar) {
        ks.hasCmd = true;
        ks.command = cmd;
    }
    process(ks);
}

void Keyboard::keyRelease(uchar code) {
    keyStatus ks;
    ks.pressed = false;
    ks.modifiers = status & 0x0F;
    
    uchar cmd = controllKeys[code];
    code &= 0x7F;
    
    if (!cmd)
        cmd = controllKeys[code];
    
    if (!cmd) {
        ks.hasChar = true;
        ks.character = 0;
        
        if ((status & STATUS_ALT) || (status & STATUS_CTRL))
            ks.hasCmd = true;
        
        if ((status & STATUS_SHIFT) ^ (status & STATUS_CAPS)) {
            if (status & STATUS_ALTGR)
                ks.character = keymapNormal[code]; //keymapShiftAltgr
            else {
                if (status & STATUS_CAPS)
                    ks.character = keymapCaps[code];
                else
                    ks.character = keymapShift[code];
            }
        } else {
            if (status & STATUS_ALTGR)
                ks.character = keymapNormal[code]; //keymapAltGr
            else 
                ks.character = keymapNormal[code];
        }
    } else if (cmd >= KBDC_KPINSERT && cmd <= KBDC_KPDEL && (status & STATUS_NUM)) {
        ks.hasChar = true;
        if ((status & STATUS_ALT) || (status & STATUS_CTRL))
            ks.hasCmd = true;
        
        if (cmd == KBDC_KPDEL)
            ks.character = (uint)'.';
        else
            ks.character = (uint)'0' + (cmd - KBDC_KPINSERT);
    } else if (cmd == KBDC_ALT)
        status &= ~STATUS_ALT;
    else if (cmd == KBDC_ALTGR)
        status &= ~STATUS_ALTGR;
    else if (cmd == KBDC_LEFTCTRL || cmd == KBDC_RIGHTCTRL)
        status &= ~STATUS_CTRL;
    else if (cmd == KBDC_LEFTSHIFT || cmd == KBDC_RIGHTSHIFT)
        status &= ~STATUS_SHIFT;
     
    if (!ks.hasChar) {
        ks.hasCmd = true;
        ks.command = cmd;
    }
    process(ks);
}

void Keyboard::process(const keyStatus& ks) {
    
}