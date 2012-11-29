#ifndef DEVICE_PH
#define DEVICE_PH

#include <Library/String.h>
#include <SyscallManager/IDT.h>

class DeviceProto {
public:
    virtual String getName() const = 0;
    virtual String getClass() const = 0;
    
    virtual void IRQHandler(IDT::regs*) { }
};

#endif