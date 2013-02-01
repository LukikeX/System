#include "GraphicDisplay.proto.h"

char GraphicDisplay::consoleFont[256][16];

uint GraphicDisplay::consoleColor[] = {
    0x00000000,		// 0 == black
    0x000000AA,		// 1 == dark blue
    0x0000AA00,		// 2 == dark green
    0x0000AAAA, 	// 3 == dark cyan
    0x00AA0000,		// 4 == dark red
    0x00AA00AA,		// 5 == dark magenta
    0x00AA5500,		// 6 == dark orange
    0x00A0A0A0,		// 7 == light grey
    0x00555555,		// 8 == dark grey
    0x005555FF,		// 9 == bright blue
    0x0055FF55,		// A == bright green
    0x0055FFFF,		// B == bright cyan
    0x00FF5555,		// C == bright red
    0x00FF55FF,		// D == bright yellow
    0x00FFFF00,		// E == bright orange
    0x00FFFFFF,		// F == white  
};

void GraphicDisplay::putChar(ushort line, ushort col, WChar c, uchar color) {
    drawChar(line, col, c, color);
    if (line == csrBuff.line && col == csrBuff.col) {
        getCsrBuff();
        drawCsr();
    }
}

void GraphicDisplay::drawChar(ushort line, ushort col, WChar c, uchar color) {
    uchar ch = c.toAscii();
    if (ch)
        return;
    
    ushort sx = col * 9, sy = line * 16;
    uint fgColor = consoleColor[color & 0xF], bgColor = consoleColor[(color >> 4) & 0xF];
    
    for (uint i = 0; i < 16; i++) {
        uchar pixs = consoleFont[ch][i];
        for (uint j = 7; j; j--) {
            putPixel(sx + j, sy + i, pixs & 1 ? fgColor : bgColor);
            pixs >>= 1;
        }
        putPixel(sx + 8, sy + i, bgColor);
    }
}

void GraphicDisplay::moveCursor(ushort line, ushort col) {
    putCrsBuff();
    csrBuff.line = line;
    csrBuff.col = col;
    getCsrBuff();
    drawCsr();
}

void GraphicDisplay::getCsrBuff() {
    if (csrBuff.col < 0 || csrBuff.line < 0)
        return;
    
    for (uint x = 0; x < 9; x++) {
        for (uint y = 0; y < 16; y++)
            csrBuff.buff[x][y] = getPixel(csrBuff.col * 9 + x, csrBuff.line * 16 + y);
    }
}

void GraphicDisplay::putCrsBuff() {
    if (csrBuff.col < 0 || csrBuff.line < 0)
        return;
    
    for (uint x = 0; x < 9; x++) {
        for (uint y = 0; y < 16; y++)
            putPixel(csrBuff.col * 9 + x, csrBuff.line * 16 + y, csrBuff.buff[x][y]);
    }
}

void GraphicDisplay::drawCsr() {
    if (csrBuff.col < 0 || csrBuff.line < 0)
        return;
    
    for (uint x = 0; x < 9; x++) {
        putPixel(csrBuff.col * 9 + x, csrBuff.line * 16 + 14, 0x00FFFFFF);
        putPixel(csrBuff.col * 9 + x, csrBuff.line * 16 + 15, 0x00000000);
    }
}