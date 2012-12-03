#include "Device.h"
#include <Devices/Device.proto.h>

Vector<DeviceProto *> Device::devices;
DeviceProto* Device::IRQHandler[16];

Device::Device() {
    for (uint i = 0; i < 16; i++)
        IRQHandler[i] = 0;
}

void Device::registerDevice(DeviceProto* dev) {
    unregisterDevice(dev);
    devices.push(dev);
}

void Device::unregisterDevice(DeviceProto* dev) {
    for (uint i = 0; i < devices.size(); i++) {
        if (devices[i] == dev) {
            devices[i] = devices.back();
            devices.pop();
            return;
        }
    }
}

bool Device::requestIRQ(DeviceProto* dev, uint irq) {
    if (!IRQHandler[irq]) {
        IRQHandler[irq] = dev;
        return true;
    } else
        return false;
}

Vector<DeviceProto *> Device::findDevices(const String& className) {
    if (className.empty())
        return devices;
    
    Vector<DeviceProto *> ret;
    
    for (uint i = 0; i < devices.size(); i++) {
        String devClass = devices[i]->getClass();
        
        if (devClass == className)
            ret.push(devices[i]);
        else if (devClass.size() > className.size() && devClass.substr(0, className.size()) == className)
            ret.push(devices[i]);
    }
    return ret;
}

DeviceProto* Device::findDevice(String className, uint idx) {
    for (uint i = 0; i < devices.size(); i++) {
        String devClass = devices[i]->getClass();
        
        if (devClass == className || (devClass.size() > className.size() && devClass.substr(0, className.size()) == className)) {
            if (!idx)
                return devices[i];
            else
                idx--;
        }
    }
    return 0;
}

void Device::handler(IDT::regs* r) {
    if (IRQHandler[r->intNo - 32])
        ((DeviceProto *)IRQHandler[r->intNo - 32])->IRQHandler(r);
}