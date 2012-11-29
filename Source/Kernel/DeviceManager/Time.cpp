#include "Time.h"

Timer* Time::timer;

void Time::setTimer(Timer* t) {
    timer = t;
}

uint Time::uptime() {
    return timer->uptime();
}

uint Time::time() {
    return timer->time();
}