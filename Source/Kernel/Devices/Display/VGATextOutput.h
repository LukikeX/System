#ifndef VGATEXTOUTPUT_H
#define VGATEXTOUTPUT_H

#include "Display.proto.h"
#include <DeviceManager/Display.h>

#define RAM_ADDR 0xFFFFFFFFC00B8000

class VGATextoutput : public DisplayProto {
private:
    int cols;
    
public:
    VGATextoutput() : cols(80) { }
    
    String getName() const { return "Standard VGA text display"; }
    String getClass() const { return "display.text"; }
    
    void getModes(Vector<Display::modeT>& to);
    bool setMode(Display::modeT& mode);
    
    void putChar(ushort line, ushort col, WChar c, uchar color);
    void moveCursor(ushort line, ushort col);
    void clear();
    
    bool textScroll(ushort line, ushort col, ushort height, ushort width, uchar color);
};

#endif