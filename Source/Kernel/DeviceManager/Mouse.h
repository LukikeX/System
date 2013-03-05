#ifndef DM_MOUSE_H
#define DM_MOUSE_H

#include <DeviceManager/Display.h>

class Mouse {
private:
    static bool btn[3];
    static int x, y;
    static float speed;
    
public:
    struct state_T {
        bool btn[3];
        int x, y;
    };
    
    static void setBtns(char btns);
    static void SetX(char x);
    static void setY(char y);
    static state_T getState();
    
    static void setSpeed(float speed);
};

#endif