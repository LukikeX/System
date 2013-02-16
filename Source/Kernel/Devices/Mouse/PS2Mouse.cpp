#include "PS2Mouse.h"

PS2Mouse::PS2Mouse() { 
    x = 0;
    y = 0;
    no = 0;
    left = 0;
    right = 0;
    
    IO::cli();
    wait(1);
    IO::outB(0x64, 0xA8);
    wait(1);
    IO::outB(0x64, 0x20);
    wait(0);
    uchar status = IO::inB(0x60) | 2;
    wait(1);
    IO::outB(0x64, 0x60);
    wait(1);
    IO::outB(0x60, status);
    write(0xF6);
    read();
    write(0xF4);
    read();
    IO::sti();
    
    Device::requestIRQ(this, 12);
    Device::registerDevice(this);
}

void PS2Mouse::wait(char type) {
    for (uint i = 0; i < 100000; i++) {
        if ((IO::inB(0x64) & (type ? 1 : 2)) == (type ? 1 : 0))
            return;
    }
}

void PS2Mouse::write(char str) {
    wait(1);
    IO::outB(0x64, 0xD4);
    wait(1);
    IO::outB(0x60, str);
}

char PS2Mouse::read() {
    wait(0);
    return IO::inB(0x60);
}

PS2Mouse::state_T PS2Mouse::getState() const {
    state_T ret;
    ret.x = x;
    ret.y = y;
    ret.left = left;
    ret.right = right;
    return ret;
}

void PS2Mouse::IRQHandler(IDT::regs*) {
    if (no++ == 3)
        no = 0;
    
    char s = IO::inB(0x60);
    switch (no) {
        case 0:
            left = s & 1;
            right = s & 2;
            break;
        case 1:
            x += s;
            if (x > Display::graphWidth())
                x = Display::graphWidth();
            else if (x < 0)
                x = 0;
            break;
        case 2:
            y += s;
            
            if (y > Display::graphHeight())
                y = Display::graphHeight();
            else if (y < 0)
                y = 0;
            break;
    }
    
    int color;
    if (left & right)
        color = 0xFFFF00;
    else if (left)
        color = 0xFF0000;
    else if (right)
        color = 0x00FF00;
    else
        color = 0x0000FF;
    
    for (uint i = 0; i < 10; i++) {
        for (uint j = 0; j < 10; j++)
            Display::putPixel(x + i, y + j, color);
    }
}