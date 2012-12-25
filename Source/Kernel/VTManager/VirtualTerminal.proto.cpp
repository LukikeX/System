#include "VirtualTerminal.proto.h"

VirtualTerminal::VirtualTerminal() : Ressource(VTIF_OBJTYPE, VirtualTerminal::callTable), keyboardMutex(MUTEX_FALSE) {
    kbdBufferStart = 0;
    kbdBufferEnd = 0;
}

VirtualTerminal::~VirtualTerminal() { }

void VirtualTerminal::write(const String& s, bool updateScr) {
    for (uint i = 0; i < s.size(); i++) {
        if (s[i] == WChar(MVT_ESC)) {
            mvtEscCmdT cmd;
            cmd.cmd = s[i + 1];
            cmd.a = s[i + 2];
            cmd.b = s[i + 3];
            i += 3;
            handleEscape(cmd);
        } else
            put(s[i], false);
    }
    
    if (updateScr)
        updateCursor();
}

void VirtualTerminal::writeDec(long num, bool updateScr) {
    ulong i = num;
    if (!i)
        put('0', false);
    else if (num < 0) {
        put('-', false);
        i = 0 - num;
    }
    char c[32];
    int n = 0;
    while (i > 0) {
        c[n] = '0' + (i % 10);
        i /= 10;
        n++;
    }
    
    while (n > 0) {
        n--;
        put(c[n], false);
    }
    
    if (updateScr)
        updateCursor();
}

void VirtualTerminal::writeHex(ulong num, bool updateScr) {
    write("0x", false);
    char hexdigits[] = "0123456789ABCDEF";
    for (uint i = 0; i < 16; i++) {
        put(hexdigits[(num & 0xF000000000000000) >> 60], false);
        num = num << 4;
    }
    
    if (updateScr)
        updateCursor();
}

void VirtualTerminal::writeHex(uint num, bool updateScr) {
    write("0x", false);
    char hexdigits[] = "0123456789ABCDEF";
    for (uint i = 0; i < 8; i++) {
        put(hexdigits[(num & 0xF0000000) >> 28], false);
        num = num << 4;
    }
    
    if (updateScr)
        updateCursor();
}

void VirtualTerminal::writeHex(ushort num, bool updateScr) {
    write("0x", false);
    char hexdigits[] = "0123456789ABCDEF";
    for (uint i = 0; i < 4; i++) {
        put(hexdigits[(num & 0xF000) >> 12], false);
        num = num << 4;
    }
    
    if (updateScr)
        updateCursor();
}