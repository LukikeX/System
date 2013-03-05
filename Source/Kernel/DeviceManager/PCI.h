#ifndef PCI_H
#define PCI_H

#include <Library/Vector.h>
#include <Devices/PCI/PCIDev.h>

class PCI {
private:
    static Vector<PCIDev *> list;
public:
    PCI();
    static void deviceList();
    static void scanPCI();
    static bool isPresent();
    
    static const char* class2String(uchar classCode, uchar subClass);
    static const char* vendor2String(ushort id);
    static const char* vendorDevice2String(ushort vendor, ushort device);
    
    static PCIDev* getDev(ushort vendor, ushort device);
    static PCIDev* getDevByClass(uchar classcode);
};

#endif