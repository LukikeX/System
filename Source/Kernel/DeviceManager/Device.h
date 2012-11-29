#ifndef DM_DEVICE_H
#define DM_DEVICE_H

#include <Library/String.h>
#include <Library/Vector.h>
#include <SyscallManager/IDT.h>

class DeviceProto;

class Device {
private:
    static Vector<DeviceProto *> devices;
    static DeviceProto* IRQHandler[16];
    
public:
    Device();
    static void handler(IDT::regs* r);
    static void registerDevice(DeviceProto* dev);
    static void unregisterDevice(DeviceProto* dev);
    static bool requestIRQ(DeviceProto* dev, uint irq);
    
    static Vector<DeviceProto *> findDevices(const String& className = "");
    DeviceProto* findDevice(String className = "", uint idx = 0);
};


#endif