#ifndef VIRTUALTERMINAL_PROTO_H
#define VIRTUALTERMINAL_PROTO_H

#include <Core/Mutex.h>
#include <Library/WChar.h>
#include <Devices/Keyboard/Keyboard.defs.h>
#include <Library/String.h>

#include "VirtualTerminal.def.h"

#define EOF 0x12

class VirtualTerminal {
protected:
    Mutex keyboardMutex;
    keyStatus kbdBuffer[32];
    int kbdBufferStart, kbdBufferEnd;
    
    virtual void updateCursor() { }
    
public:
    VirtualTerminal();
    virtual ~VirtualTerminal();
    
    virtual bool isBoxed() = 0;
    virtual uchar height() { return 0; }
    virtual uchar width() { return 0; }
    
    virtual void handleEscape(mvtEscCmdT cmd) = 0;
    
    virtual void put(WChar c, bool updateScr = true) = 0;
    void write(const String& s, bool updateScr = true);
    
    //for kernel use
    void writeDec(long num, bool updateScr = true);
    void writeHex(ushort num, bool updateScr = true);
    void writeHex(uint num, bool updateScr = true);
    void writeHex(ulong num, bool updateScr = true);
    
    inline VirtualTerminal& operator<< (const String& s) { write(s); return *this; }
    inline VirtualTerminal& operator<< (int i) { writeDec(i); return *this; }
    inline VirtualTerminal& operator<< (long i) { writeDec(i); return *this; }
    inline VirtualTerminal& operator<< (ushort i) { writeHex(i); return *this; }
    inline VirtualTerminal& operator<< (uint i) { writeHex(i); return *this; }
    inline VirtualTerminal& operator<< (ulong i) { writeHex(i); return *this; }
    
    //keyboard func
    virtual void keyPress(keyStatus ks);
    virtual keyStatus getkeyPress(bool show = true, bool block = true);
    String readLine(bool show = true);
    
    struct vtchr {
        uchar color;
        WChar c;
    };
};

#endif