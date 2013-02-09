#ifndef DM_DISPLAY_H
#define DM_DISPLAY_H

#include <Library/Vector.h>
#include <Library/WChar.h>

class DisplayProto;
class VGATextoutput;

class Display {
public:
    struct modeT {
        int textCols, textRows;
        int graphWidth, graphHeight, graphDepth;
        int identifier;
        DisplayProto* device;
    };
    
    static Vector<modeT> modes;
    static modeT mode;
    
    inline static ushort textCols()    { return mode.textCols; }
    inline static ushort textRows()    { return mode.textRows; }
    inline static ushort graphWidth()  { return mode.graphWidth; }
    inline static ushort graphHeight() { return mode.graphHeight; }
    inline static ushort graphDepth()  { return mode.graphDepth; }
    static void clear();
    
    static void putChar(ushort line, ushort col, WChar c, uchar color);
    static void moveCursor(ushort line, ushort col);
    static bool textScroll(ushort line, ushort col, ushort height, ushort width, uchar color);
    static void putPixel(ushort x, ushort y, uint color);
    static uint getPixel(ushort x, ushort y);
    
    static void getModes();
    static bool setMode(modeT& cMode);
    
    static void setText(VGATextoutput* out);
};

#endif