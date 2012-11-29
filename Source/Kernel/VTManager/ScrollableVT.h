#ifndef SCROLLABLEVT_H
#define SCROLLABLEVT_H

#include "SimpleVT.h"

class ScrollableVT : public SimpleVT {
private:
    vtchr** lines;
    uint keepRows, linesUp;
    
    void updateCursor();
    void redraw();
    void scroll();
    
public:
    ScrollableVT(uint rows, uint cols, uint keepRows, uchar fgColor = 7, uchar bgColor = 0);
    virtual ~ScrollableVT();
    
    virtual void putChar(uint row, uint col, WChar c);
    void keyPress(keyStatus ks);
};

#endif