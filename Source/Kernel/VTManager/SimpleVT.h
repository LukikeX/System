#ifndef SIMPLEVT_H
#define SIMPLEVT_H

#include "VirtualTerminal.proto.h"

class VT;

class SimpleVT : public VirtualTerminal {
    friend class VT;
protected:
    vtchr* buff;
    uint rows, cols;
    uchar color;
    
    uint maprow, mapcol;
    bool mapped, hideCursor;
    
    uint csrlin, csrcol;
    
    virtual void updateCursor();
    virtual void clear();
    virtual void redraw();
    virtual void scroll();
    
public:
    SimpleVT(uint rows, uint cols, uchar fgColor = 7, uchar bgColor = 0);
    virtual ~SimpleVT();
    
    virtual void putChar(uint row, uint col, WChar c);
    virtual void handleEscape(mvtEscCmdT cmd);
    
    bool isBoxed() { return true; }
    uchar height() { return rows; }
    uchar width()  { return cols; }
    uchar csrLin() { return csrlin; }
    uchar csrCol() { return csrcol; }
    void setColor(uchar color) { this->color = color; }
    
    void map(int row = -1, int col = -1);
    void unmap();
    
    void put(WChar c, bool updateCsr = true);
};

#endif