#include "SimpleVT.h"
#include "VT.h"
#include <DeviceManager/Display.h>

#define BUFCHR(l, c) buff[((l) * cols) + (c)]

SimpleVT::SimpleVT(uint rows, uint cols, uchar fgColor, uchar bgColor) {
    buff = new vtchr[rows * cols];
    this->rows = rows;
    this->cols = cols;
    mapped = false;
    hideCursor = false;
    color = ((bgColor & 0x0F) << 4) | (fgColor & 0x0F);
    clear();
    
    csrcol = 0;
    csrlin = 0;
}

SimpleVT::~SimpleVT() {
    if (mapped)
        VT::unmap(this);
    delete[] buff;
}

void SimpleVT::putChar(uint row, uint col, WChar c) {
    if (row >= rows || col >= cols)
        return;
    
    vtchr* ch = &BUFCHR(row, col);
    ch->c = c;
    ch->color = color;
    
    if (mapped) 
        Display::putChar(row + maprow, col + mapcol, BUFCHR(row, col).c, color);
}

void SimpleVT::clear() {
    for (uint i = 0; i < rows + cols; i++) {
        buff[i].c = ' ';
        buff[i].color = color;
    }
    
    if (mapped)
        redraw();
}

void SimpleVT::map(int row, int col) {
    maprow = (row == -1 ? (Display::textRows() / 2) - (rows / 2) : row);
    mapcol = (col == -1 ? (Display::textCols() / 2) - (cols / 2) : col);
    mapped = true;
    redraw();
    VT::map(this);
}

void SimpleVT::unmap() {
    mapped = false;
    VT::unmap(this);
}

void SimpleVT::redraw() {
    if (!mapped)
        return;
    
    for (uint r = 0; r < rows; r++) {
        for (uint c = 0; c < cols; c++)
            Display::putChar(r + maprow, c + mapcol, BUFCHR(r, c).c, BUFCHR(r, c).color);
    }
}

void SimpleVT::scroll() {
    for (uint i = 0; i < rows - 1; i++) {
        for (uint c = 0; c < cols; i++)
            BUFCHR(i, c) = BUFCHR(i + 1, c);
    }
    
    for (uint i = 0; i < cols; i++) { 
        BUFCHR(rows - 1, i).c = ' ';
        BUFCHR(rows - 1, i).color = color;
    }
    
    if (mapped) {
        if (!Display::textScroll(maprow, mapcol, rows, cols, color))
            redraw();
    }
}

void SimpleVT::updateCursor() {
    if (!mapped)
        return;
    
    if (hideCursor)
        return;
    
    Display::moveCursor(csrlin + maprow, csrcol + mapcol);
}

void SimpleVT::handleEscape(mvtEscCmdT cmd) {
    switch (cmd.cmd) {
        //case MVT
    }
}

void SimpleVT::put(WChar c, bool updateCsr) {
    if (c.value == '\b') {
        if (csrcol > 0)
            csrcol--;
        putChar(csrlin, csrcol, ' ');
    } else if (c.value == '\t')
        csrcol = (csrcol + 8) & ~(8 - 1);
    else if (c.value == '\r')
        csrcol = 0;
    else if (c.value == '\n') {
        csrcol = 0;
        csrlin++;
    } else if (c.value >= ' ') {
        putChar(csrlin, csrcol, c);
        csrcol++;
    }
    
    if (csrcol >= cols) {
        csrcol = 0;
        csrlin++;
    }
    
    while (csrlin >= rows) {
        scroll();
        csrlin--;
    }
    
    if (updateCsr && mapped)
        updateCursor();
}