#include "VGATextOutput.h"
#include <Core/IO.h>

void VGATextoutput::getModes(Vector<Display::modeT>& to) {
    Display::modeT m;
    m.textCols    = 40;
    m.textRows    = 25;
    m.identifier  = 1;
    m.graphWidth  = 0;
    m.graphHeight = 0;
    m.graphDepth  = 0;
    m.device      = this;
    to.push(m);
    
    m.textCols    = 80;
    m.identifier  = 3;
    to.push(m);
}

bool VGATextoutput::setMode(Display::modeT& mode) {
    if (mode.device == this && (mode.identifier == 3 || mode.identifier == 1)) {
        
    }
    return false;
}

void VGATextoutput::putChar(ushort line, ushort col, WChar c, uchar color) {
    ushort* where = (ushort *)RAM_ADDR;
    where[cols * line + col] = (color << 8) | c.toAscii();
}

void VGATextoutput::moveCursor(ushort line, ushort col) {
    ushort csrLoc = line * cols + col;
    IO::outB(0x3D4, 14);
    IO::outB(0x3D5, csrLoc >> 8);
    IO::outB(0x3D4, 15);
    IO::outB(0x3D5, csrLoc & 0xFF);
}

void VGATextoutput::clear() {
    Memory::clear((ushort *)RAM_ADDR, 25 * 80);
}

bool VGATextoutput::textScroll(ushort line, ushort col, ushort height, ushort width, uchar color) {
    uchar* where = (uchar *)RAM_ADDR;
    
    for (uint i = 1; i < height; i++)
        Memory::copy(where + ((line + i) * (this->cols * 2)) + (col * 2), where + ((line + i - 1) * (this->cols * 2)) + (col * 2), width * 2);
    
    ushort* w = (ushort *)where;    
    for (uint i = 0; i < width; i++)
        (w + ((line + height - 1) * this->cols) + col)[i] = 0x20 | (color << 8);
    return true;
}