#ifndef TIME_H
#define TIME_H

#include <Devices/Timer.h>

class Time {
private:
    static Timer* timer;
public:
    static void setTimer(Timer* t);
    
    static uint uptime();
    static uint time();
};

#endif