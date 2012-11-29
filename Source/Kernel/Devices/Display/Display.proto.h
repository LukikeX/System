#ifndef DISPLAY_PH
#define DISPLAY_PH

#include <Devices/Device.proto.h>
#include <Library/WChar.h>
#include <DeviceManager/Display.h>

class DisplayProto : public DeviceProto {
public:
    virtual ~DisplayProto() { }
    
    virtual void getModes(Vector<Display::modeT>& to) = 0;
    virtual bool setMode(Display::modeT& mode) = 0;
    virtual void unsetMode() { }
    virtual void clear() = 0;
    
    virtual void putChar(ushort lie, ushort col, WChar c, uchar color) = 0;
    virtual void moveCursor(ushort line, ushort col) = 0;
    virtual bool textScroll(ushort, ushort, ushort, ushort, uchar) { return false; }
    
    virtual void putPixel(ushort, ushort, uint) { }
    virtual uint getPixel(ushort, ushort) { return 0; }
};

#endif