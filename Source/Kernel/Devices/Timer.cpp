#include "Timer.h"
#include <Core/IO.h>
#include <DeviceManager/Device.h>
#include <DeviceManager/Time.h>

Timer::Timer(uchar frequency) {
    ticks = 0;
    seconds = 0;
    
    Device::requestIRQ(this, 0);
    Time::setTimer(this);
    setFrequency(frequency);
}

void Timer::setFrequency(uchar frequency) {
    this->frequency = frequency;
    
    uint divisor = PIT_FREQENCY / (uint)frequency;
    IO::outB(PIT_CMD, 0x36);
    IO::outB(CHANNEL_0, divisor & 0xFF);
    IO::outB(CHANNEL_0, divisor >> 8);
}

void Timer::IRQHandler(IDT::regs*) {
    ticks++;
    if (ticks == frequency) {
        ticks = 0;
        seconds++;
    }
}