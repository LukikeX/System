#ifndef GRAPHICDISPLAY_PROTO_H
#define GRAPHICDISPLAY_PROTO_H

#include "Display.proto.h"

class GraphicDisplay : public DisplayProto {
private: 
    static char consoleFont[256][16];
    static uint consoleColor[16];
    
protected:
    struct {
        int line, col;
        uint buff[9][16]; // 8 + 1 padding
    } csrBuff;
    
    void getCsrBuff();
    void putCrsBuff();
    void drawCsr();
    
    public:
        GraphicDisplay() { csrBuff.line = -1; csrBuff.col = -1; }
        virtual void putChar(ushort line, ushort col, WChar c, uchar color);
        virtual void drawChar(ushort line, ushort col, WChar c, uchar color);
        
        virtual void moveCursor(ushort line, ushort col);
};

#endif