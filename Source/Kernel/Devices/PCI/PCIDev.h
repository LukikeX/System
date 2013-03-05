#ifndef PCIDEV_H
#define PCIDEV_H

#include <Core/typedef.h>
#include <Devices/Device.proto.h>

#define PCI_BASE_REG 0xCFC
#define PCI_DATA_REG 0xCF8
#define PCI_IRQ_LINE 0x3C
#define PCI_IRQ_PIN  0x3D

#define PCI_BASE_ADDRESS_0              0x10
#define PCI_BASE_ADDRESS_SPACE          0x01
#define PCI_BASE_ADDRESS_SPACE_MEMORY   0x00
#define PCI_IO_RESOURCE_MEM             0x00
#define PCI_IO_RESOURCE_IO              0x01
#define PCI_BASE_ADDRESS_MEM_MASK       (~0x0)
#define PCI_BASE_ADDRESS_IO_MASK        (~0x03)

class PCI;

class PCIDev : DeviceProto {
    friend class PCI;
public:
   struct common_T {
        ushort vendor_id;
        ushort device_id;
        ushort cmd_reg;
        ushort status_reg;
        uchar revision_id;
        uchar prog_if;
        uchar subclass;
        uchar classcode;
        uchar cacheline_size;
        uchar latency;
        uchar header_type;
        uchar BIST;
    } __attribute__((packed));
    
    struct nonBridge_T {
        ulong base_address0;
        ulong base_address1;
        ulong base_address2;
        ulong base_address3;
        ulong base_address4;
        ulong base_address5;
        ulong cardBus_CIS;
        ushort subsystem_vendorID;
        ushort subsystem_deviceID;
        ulong expansion_ROM;
        uchar cap_ptr;
        uchar reserved1[3];
        ulong reserved2[1];
        uchar interrupt_line;
        uchar interrupt_pin;
        uchar min_grant;
        uchar max_latency;
        ulong device_specific[48];
    } __attribute__((packed));
    
    struct bridge_T {
        ulong base_address0;
        ulong base_address1;
        uchar primary_bus;
        uchar secondary_bus;
        uchar subordinate_bus;
        uchar secondary_latency;
        uchar IO_base_low;
        uchar IO_limit_low;
        ushort secondary_status;
        ushort memory_base_low;
        ushort memory_limit_low;
        ushort prefetch_base_low;
        ushort prefetch_limit_low;
        ulong prefetch_base_high;
        ulong prefetch_limit_high;
        ushort IO_base_high;
        ushort IO_limit_high;
        ulong reserved2[1];
        ulong expansion_ROM;
        uchar interrupt_line;
        uchar interrupt_pin;
        ushort bridge_control;
        ulong device_specific[48];
    } __attribute__((packed));
    
    struct cardbus_T {
        ulong ExCa_base;
        uchar cap_ptr;
        uchar reserved05;
        ushort secondary_status;
        uchar PCI_bus;
        uchar cardBus_bus;
        uchar subordinate_bus;
        uchar latency_timer;
        ulong memory_base0;
        ulong memory_limit0;
        ulong memory_base1;
        ulong memory_limit1;
        ushort IObase_0low;
        ushort IObase_0high;
        ushort IOlimit_0low;
        ushort IOlimit_0high;
        ushort IObase_1low;
        ushort IObase_1high;
        ushort IOlimit_1low;
        ushort IOlimit_1high;
        uchar interrupt_line;
        uchar interrupt_pin;
        ushort bridge_control;
        ushort subsystem_vendorID;
        ushort subsystem_deviceID;
        ulong legacy_baseaddr;
        ulong cardbus_reserved[14];
        ulong vendor_specific[32];
    }__attribute((packed));
    
    struct confadd_T {
        uchar reg:8;
        uchar func:3;
        uchar dev:5;
        uchar bus:8;
        uchar rsvd:7;
        uchar enable:1;
    };
    
    enum barType {
        BAR_TYPE_MEM = 0,
        BAR_TYPE_IO
    };
    
private:
    uint bus, dev, func;
    common_T* common;
    uint devi[60];
    
    uchar irq;
    uint base[6];
    uint size[6];
    uchar type[6];
    
public:
    PCIDev(uint bus, uint dev, uint func, common_T* cmn);
    uint read(int reg, int ts);
    void write(int reg, uint value, int ts);
    
    inline uchar readB(int reg) { return read(reg, sizeof(uchar)); }
    inline ushort readW(int reg) { return read(reg, sizeof(ushort)); }
    inline uint readL(int reg) { return read(reg, sizeof(uint)); }
    
    inline void writeB(int reg, uchar val) { write(reg, val, sizeof(uchar)); }
    inline void writeW(int reg, ushort val) { write(reg, val, sizeof(ushort)); }
    inline void writeL(int reg, uint val) { write(reg, val, sizeof(uint)); }
    
    barType getBarType(int barNum);
    uint getBar(int barNum);
    inline uchar getIRQ() { return irq; }
    inline uint getBase(uchar i) { return base[i]; }
    
    uint getSize(uint base, uint mask);
    void readBases(uint count);
    
    String getName() const { return "Test PCI device"; }
    String getClass() const { return "pci.device"; }
};

#endif