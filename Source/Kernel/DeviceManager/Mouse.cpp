#include "Mouse.h"

bool Mouse::btn[3] = {0};
int Mouse::x = 0, Mouse::y = 0;
float Mouse::speed = 1.0;

void Mouse::setBtns(char btns) {
    btn[0] = btns & 1;
    btn[1] = btns & 2;
    btn[2] = btns & 3;
}

void Mouse::SetX(char x) {
    Mouse::x += x * speed;
    if (Mouse::x > Display::graphWidth())
        Mouse::x = Display::graphWidth();
    else if (Mouse::x < 0)
        Mouse::x = 0;
}

void Mouse::setY(char y) {
    Mouse::y -= y * speed;
            
    if (Mouse::y > Display::graphHeight())
        Mouse::y = Display::graphHeight();
    else if (Mouse::y < 0)
        Mouse::y = 0;
    
    
    
    
    int color;
    if (btn[0] & btn[1])
        color = 0xFFFF00;
    else if (btn[0])
        color = 0xFF0000;
    else if (btn[1])
        color = 0x00FF00;
    else
        color = 0x0000FF;
    
    for (uint i = 0 ; i < 10; i++)
        for (uint j = 0; j < 10; j++)
            Display::putPixel(x + i, Mouse::y + j, color);
}

Mouse::state_T Mouse::getState() {
    state_T ret;
    ret.btn[0] = btn[0];
    ret.btn[1] = btn[1];
    ret.btn[2] = btn[2];
    
    ret.x = x;
    ret.y = y;
    
    return ret;
}

void Mouse::setSpeed(float speed) {
    Mouse::speed = speed;
}