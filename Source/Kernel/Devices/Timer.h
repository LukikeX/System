#ifndef TIMER_H
#define	TIMER_H

#include "Device.proto.h"
#include <Library/String.h>
#include <SyscallManager/IDT.h>

#define PIT_FREQENCY 1193180

#define CHANNEL_0    0x40
#define CHANNEL_1    0x41
#define CHANNEL_2    0x42
#define PIT_CMD      0x43

class Timer : public DeviceProto {
private:
    uchar frequency;
    uchar ticks;
    uint seconds;
    
public:
    Timer(uchar frequency = 100);
    
    void setFrequency(uchar frequency);
    
    String getName() const { return "timer"; }
    String getClass() const { return "Programmable interrupt timer"; }
    
    inline uint uptime() { return seconds; }
    inline uint time() { return (seconds * 1000) + ((ticks * 1000) / frequency); }
    
    void IRQHandler(IDT::regs*);
};

#endif