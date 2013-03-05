#include "PCI.h"
#include "Device.h"
#include <Devices/PCI/PCIDefs.h>
#include <Core/IO.h>
#include <Core/Loader.h>

Vector<PCIDev *> PCI::list = 0;

PCI::PCI() {
    if (!isPresent())
        //throw new pci bus doesn't exist...
        return;
    
    for (uint bus = 0; bus < 4; bus++) {
        for (uint dev = 0; dev < 32; dev++) {
            for (uint fun = 0; fun < 8; fun++) {
                uint* temp = new uint[4];
                PCIDev pf(bus, dev, fun, 0);
                
                for (uint i = 0; i < 4; i++)
                    temp[i] = pf.readL(i << 2);
                
                PCIDev::common_T* cfg = (PCIDev::common_T *)temp;
                if (cfg->vendor_id == 0xFFFF || cfg->vendor_id == 0) {
                    delete[] temp;
                    continue;
                }
                
                PCIDev* newDev = new PCIDev(bus, dev, fun, cfg);
                for (uint i = 0; i < 60; i++)
                    newDev->devi[i] = newDev->readL((i << 2) + 16);
                
                list.push(newDev);
            }
        }
    }
}

bool PCI::isPresent() {
    return IO::inL(PCI_DATA_REG) != 0xFFFFFFFF;
}

const char* PCI::class2String(uchar classCode, uchar subClass) {
    uint code = (classCode << 8) + subClass;
    
    switch (code) {
        case PCI_CLASS_NOT_DEFINED: return "Unknown device/VGA";
        case PCI_CLASS_NOT_DEFINED_VGA: return "Unknown/VGA";
        case PCI_CLASS_STORAGE_SCSI: return "SCSI-Disk";
        case PCI_CLASS_STORAGE_IDE: return "IDE-Disk";
        case PCI_CLASS_STORAGE_FLOPPY: return "Floppy-Disk";
        case PCI_CLASS_STORAGE_IPI: return "IPI-Disk";
        case PCI_CLASS_STORAGE_RAID: return "RAID";
        case PCI_CLASS_STORAGE_SATA: return "SATA";
        case PCI_CLASS_STORAGE_SATA_AHCI: return "SATA AHCI";
        case PCI_CLASS_STORAGE_SAS: return "SAS";
        case PCI_CLASS_STORAGE_OTHER: return "Other Storage";
        case PCI_CLASS_NETWORK_ETHERNET: return "Ethernet";	
        case PCI_CLASS_NETWORK_TOKEN_RING: return "Tokenring";	
        case PCI_CLASS_NETWORK_FDDI: return "FDDI";	
        case PCI_CLASS_NETWORK_ATM: return "ATM";	
        case PCI_CLASS_NETWORK_OTHER: return "Other";	
        case PCI_CLASS_DISPLAY_VGA: return "VGA";
        case PCI_CLASS_DISPLAY_XGA: return "XGA";	
        case PCI_CLASS_DISPLAY_3D: return "3D";	
        case PCI_CLASS_DISPLAY_OTHER: return "Other Disp";	
        case PCI_CLASS_MULTIMEDIA_VIDEO: return "Video";
        case PCI_CLASS_MULTIMEDIA_AUDIO: return "Audio";	
        case PCI_CLASS_MULTIMEDIA_PHONE: return "Phone";	
        case PCI_CLASS_MULTIMEDIA_OTHER: return "Other Mmedia";	

        case PCI_CLASS_MEMORY_RAM: return "RAM";
        case PCI_CLASS_MEMORY_FLASH: return "FLASH";
        case PCI_CLASS_MEMORY_OTHER: return "Other Mem";
        case PCI_CLASS_BRIDGE_HOST: return "Host Bridge";
        case PCI_CLASS_BRIDGE_ISA: return "ISA Bridge";
        case PCI_CLASS_BRIDGE_EISA: return "EISA Bridge";
        case PCI_CLASS_BRIDGE_MC: return "MC Bridge";
        case PCI_CLASS_BRIDGE_PCI: return "PCI Bridge";
        case PCI_CLASS_BRIDGE_PCMCIA: return "PCMCIA Bridge";
        case PCI_CLASS_BRIDGE_NUBUS: return "NUBUS Bridge";
        case PCI_CLASS_BRIDGE_CARDBUS: return "CardBus Bridge";
        case PCI_CLASS_BRIDGE_RACEWAY: return "Raceway Bridge";
        case PCI_CLASS_BRIDGE_OTHER: return "Other Bridge";
        case PCI_CLASS_COMMUNICATION_SERIAL: return "Serial";
        case PCI_CLASS_COMMUNICATION_PARALLEL: return "Parallel";
        case PCI_CLASS_COMMUNICATION_MULTISERIAL: return "MultiSerial";
        case PCI_CLASS_COMMUNICATION_MODEM: return "Modem";
        case PCI_CLASS_COMMUNICATION_OTHER: return "Other Comm";
        case PCI_CLASS_SYSTEM_PIC: return "PIC";
        case PCI_CLASS_SYSTEM_PIC_IOAPIC: return "IOAPIC";
        case PCI_CLASS_SYSTEM_PIC_IOXAPIC: return "IOXAPIC";
        case PCI_CLASS_SYSTEM_DMA: return "DMA";
        case PCI_CLASS_SYSTEM_TIMER: return "Timer";
        case PCI_CLASS_SYSTEM_RTC: return "RTC";
        case PCI_CLASS_SYSTEM_PCI_HOTPLUG: return "PCI Hotplug";
        case PCI_CLASS_SYSTEM_SDHCI: return "SDHCI";
        case PCI_CLASS_SYSTEM_OTHER: return "Other System";
        case PCI_CLASS_INPUT_KEYBOARD: return "Keyboard";
        case PCI_CLASS_INPUT_PEN: return "Pen";
        case PCI_CLASS_INPUT_MOUSE: return "Mouse";
        case PCI_CLASS_INPUT_SCANNER: return "SCANNER";
        case PCI_CLASS_INPUT_GAMEPORT: return "Gameport";
        case PCI_CLASS_INPUT_OTHER: return "Other Input";
        case PCI_CLASS_DOCKING_GENERIC: return "Docking generic";
        case PCI_CLASS_DOCKING_OTHER: return "Docking Other";
        case PCI_CLASS_PROCESSOR_386: return "i386";
        case PCI_CLASS_PROCESSOR_486: return "i486";
        case PCI_CLASS_PROCESSOR_PENTIUM: return "Pentium";
        case PCI_CLASS_PROCESSOR_ALPHA: return "Alpha";
        case PCI_CLASS_PROCESSOR_MIPS: return "MIPS";
        case PCI_CLASS_PROCESSOR_CO: return "CO???";

        case PCI_CLASS_SERIAL_FIREWIRE: return "Firewire";
        case PCI_CLASS_SERIAL_FIREWIRE_OHCI: return "Firewire-OHCI";
        case PCI_CLASS_SERIAL_SSA: return "SSA";
        case PCI_CLASS_SERIAL_USB: return "USB";
        case PCI_CLASS_SERIAL_USB_UHCI: return "USB UHCI";
        case PCI_CLASS_SERIAL_USB_OHCI: return "USB OHCI";
        case PCI_CLASS_SERIAL_USB_EHCI: return "USB EHCI";
        case PCI_CLASS_SERIAL_FIBER: return "Fiber";
        case PCI_CLASS_SERIAL_SMBUS: return "SMBUS";
        case PCI_CLASS_WIRELESS_RF_CONTROLLER: return "Wireless rf control";
        case PCI_CLASS_WIRELESS_WHCI: return "Wireless rf control";
        case PCI_CLASS_INTELLIGENT_I2O: return "Intelligent I2O";
        case PCI_CLASS_SATELLITE_TV: return "Satellite Tv";
        case PCI_CLASS_SATELLITE_AUDIO: return "Satellite Audio";
        case PCI_CLASS_SATELLITE_VOICE: return "Satellite Voice";
        case PCI_CLASS_SATELLITE_DATA: return "Satellite Data";
        case PCI_CLASS_CRYPT_NETWORK: return "Crypt Network";
        case PCI_CLASS_CRYPT_ENTERTAINMENT: return "Crypt Entertainment";
        case PCI_CLASS_CRYPT_OTHER: return "Crypt Other";
        case PCI_CLASS_SP_DPIO: return "DPIO";
        case PCI_CLASS_SP_OTHER: return "DSP Other";
        default: return "Invalid class subclass";
    }
}

const char* PCI::vendor2String(ushort id) {
    for (uint i = 0; i < PCI_VENTABLE_LEN; i++) {
        if (vendorArray[i].id == id)
            return vendorArray[i].vendor;
    }
    return "Unknown vendor";
}

const char* PCI::vendorDevice2String(ushort vendor, ushort device) {
    for (uint i = 0; i < PCI_DEVICE_TABLE_LEN; i++) {
        if (PCIbusDeviceNames[i].vendor == vendor && PCIbusDeviceNames[i].device == device)
            return PCIbusDeviceNames[i].name;
    }
    return "Unknown device";
}

void PCI::deviceList() {
    for (uint i = 0; i < list.size(); i++) {
        *kvt << (int)list[i]->bus << ":" << (int)list[i]->dev << ":" << (int)list[i]->func << ":"
             << vendor2String(list[i]->common->vendor_id) << ":"
             << vendorDevice2String(list[i]->common->vendor_id, list[i]->common->device_id) << ":"
             << class2String(list[i]->common->classcode, list[i]->common->subclass) << ":"
             << (int)list[i]->common->classcode << ":" << (int)list[i]->common->subclass << ":"
             << (int)list[i]->common->prog_if << ":" << (int)list[i]->common->header_type
             << "\n";
    }
}

PCIDev* PCI::getDev(ushort vendor, ushort device) {
    for (uint i = 0; i < list.size(); i++) {
        if (list[i]->common->vendor_id == vendor && list[i]->common->device_id == device)
            return list[i];
    }
    return 0;
}

PCIDev* PCI::getDevByClass(uchar classcode) {
    return 0;
}