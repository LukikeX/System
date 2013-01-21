#include "ScrollableVT.h"
#include <DeviceManager/Display.h>

#define BUFCHR(l, c) buff[((l) * cols) + (c)]

ScrollableVT::ScrollableVT(uint rows, uint cols, uint keepRows, uchar fgColor, uchar bgColor) :
        SimpleVT(rows, cols, fgColor, bgColor) {
    this->keepRows = keepRows;
    linesUp = 0;
    lines = new vtchr*[cols];
    
    for (uint i = 0; i < keepRows; i++) {
        lines[i] = new vtchr[cols];
        for (uint j = 0; j < cols; j++) {
            lines[i][j].color = color;
            lines[i][j].c = " ";
        }
    }
}

ScrollableVT::~ScrollableVT() {
    for (uint i = 0; i < keepRows; i++)
        delete[] lines[i];
    delete lines;
}

void ScrollableVT::putChar(uint row, uint col, WChar c) {
    if (row >= rows || col >= cols)
        return;
    
    vtchr* ch = &BUFCHR(row, col);
    ch->c = c;
    ch->color = color;
    
    if (mapped) {
        if (row + linesUp < rows)
            Display::putChar(row + maprow + linesUp, col + mapcol, BUFCHR(row, col).c, color);
    }
}

void ScrollableVT::updateCursor() {
    if (csrlin + linesUp < rows)
        Display::moveCursor(csrlin + maprow + linesUp, csrcol + mapcol);
}

void ScrollableVT::redraw() {
    if (!mapped)
        return;
    
    for (uint r = 0; r < rows; r++) {
        if (r > linesUp) {
            for (uint c = 0; c < cols; c++)
                Display::putChar(r + maprow, c + mapcol, BUFCHR(r - linesUp, c).c, BUFCHR(r - linesUp, c).color);
        } else {
            for (uint c = 0; c < cols; c++) {
                //uint l = keepRows - linesUp + r;
                //Display::putChar(r + maprow, c + mapcol, lines[l][c].c, lines[l][c].color);
            }
        }
    }
}

void ScrollableVT::scroll() {
    for (uint i = 0; i < cols; i++)
        lines[0][i] = BUFCHR(0, i);
    
    vtchr* x = lines[0];
    for (uint i = 1; i < keepRows; i++)
        lines[i - 1] = lines[i];
    
    lines[keepRows - 1] = x;
    SimpleVT::scroll();
}

void ScrollableVT::keyPress(keyStatus ks) {
    if (ks.hasCmd && ks.modifiers == STATUS_SHIFT) {
        int nlup = linesUp;
        if (ks.command == KBDC_PGUP)
            nlup = linesUp + (rows - 2);
        else if (ks.command == KBDC_PGDOWN)
            nlup = linesUp - (rows - 2);
        else if (ks.command == KBDC_UP)
            nlup = linesUp + 1;
        else if (ks.command == KBDC_DOWN)
            nlup = linesUp - 1;
        else if (ks.command == KBDC_END)
            nlup = 0;
        else
            VirtualTerminal::keyPress(ks);
        
        if (nlup < 0)
            nlup = 0;
        
        linesUp = nlup;
        
        if (linesUp > keepRows)
            linesUp = keepRows;
        redraw();
        updateCursor();
    } else
        VirtualTerminal::keyPress(ks);
}