#ifndef GDT_H
#define GDT_H

#include <Core/typedef.h>
#include <MemoryManager/Memory.h>

#define GDT_TYPE_SYSTEM 0x8
#define GDT_TYPE_TSS    (GDT_TYPE_SYSTEM | 0x1)

class GDT {
private:
    struct selectorT {
        union {
            struct {
                ushort limitLow;      // 1...16 of limit
                ushort baseLow;       // 1...16 of base
                uint baseMidLow  : 8, // 17...24 of base
                     type        : 4,
                     segSel      : 1,
                     dpl         : 2,
                     present     : 1,
                     limitHigh   : 4,
                     _av10       : 1,
                     large       : 1,
                     opsz        : 1,
                     granularity : 1,
                     baseMidHigh : 8; // 25...32 of base
            } fields;
            ulong bytes;
        };
    } __attribute__((packed));
    
    struct selectorExtT {
        union {
            struct {
                uint baseHigh; // 33...64 of base
                uint _res0  : 8,
                     _zero3 : 5,
                     _res1  : 19;
            } fields;
            ulong bytes;
        };
    };
    
    struct ptrT {
        ushort limit;
        ulong  base;
    } __attribute__((packed));
    
    struct tssEntryT {
        uint res0;
        ulong rsp[3];
        ulong res1;
        ulong ist[7];
        ulong res2;
        ushort res3;
        ushort iobm_off;
    } __attribute__((packed));
    
    static selectorT* entry;
    static tssEntryT  tssEntry;
    static ptrT*      ptr;
    
public:
    GDT();
    static void setGate(ushort sel, ulong base, uint limit, uint type, uchar dpl, bool large, bool sys);
    inline static void setTSSRSP(ulong rsp) { tssEntry.rsp[0] = rsp; }
};

#endif