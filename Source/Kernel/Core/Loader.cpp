#include "Loader.h"
#include "Devices/Timer.h"
#include "typedef.h"
#include "Panic.h"
#include "SB.h"
#include "IO.h"
#include "SyscallManager/Res.h"

#include <C++/Runtime.h>

#include <MemoryManager/PageAlloc.h>
#include <MemoryManager/Memory.h>
#include <MemoryManager/GDT.h>

#include <DeviceManager/Device.h>
#include <DeviceManager/Display.h>
#include <DeviceManager/Time.h>
#include <DeviceManager/Keyboard.h>

#include <Devices/Display/VGATextOutput.h>
#include <Devices/Keyboard/PS2Keyboard.h>

#include <VTManager/SimpleVT.h>
#include <VTManager/ScrollableVT.h>

#include <TaskManager/Task.h>

//basicstring uvolnit pamet v destructore
//ScrollableVT - redraw().... opravit
//String number
//Opravit bitset, nejako vynechava bity
//Dorobit v thread accessible
//Doborbit v keymape altgr a shiftaltgr
//dorobit v file seek
//vo FSNode readable, writable atd.
//dorobit mount vo VFS
//dorobit DMA na pracu s grafikou

//Process:
//run()
//scall - uid
//Dorobit par detailov v syscall

/* Syscalls port
 * syscall - 64
 * request task switch - 65
 * signal that thread ended - 66
 */

SimpleVT* kvt;

 ulong syscall(ulong n, ulong a, ulong b, ulong c, ulong d, ulong e) {
    ulong r;
    asm volatile ("int $64" : "=a"(r) : "a"(n), "b"(a), "c"(b), "d"(c), "D"(d), "S"(e));
    return r;
}

void prog1() {
    ulong ret = syscall(0xFFFFFFFE00000000 | VTIF_SGETPROUTVT, VTIF_OBJTYPE, 0, 0, 0, 0);
    
    String *s = new String;
    *s = "test";
    
    syscall(ret << 32 | VTIF_WRITE, (ulong)s, 0, 0, 0, 0);
}


void Print(header_T* h);

extern "C" void Loader(header_T* header) {
    Memory::placementAddress = (ulong)&_end;
    construct();
    Res::size = 0;
    
    VGATextoutput* vgaout = new VGATextoutput();
    Display::setText(vgaout);
    
    SB* sb = new SB(11);
    SB::progress("Initializing paging...");
    ulong size = 0x100000;
    for (uint i = 0; i < header->mapLen; i++)
        if (header->memMap->base == size)
            size += header->memMap->length;
    
    PhysMem(5);
    SB::ok();
    
    SB::progress("Initializing free pages...");
    PageAlloc();
    SB::ok();

    SB::progress("Initializing memory heap...");
    Memory();
    SB::ok();
    
    SB::progress("Initializing GDT...");
    GDT();
    SB::ok();

    SB::progress("Creating Kernel VT...");
    //kvt = new ScrollableVT(Display::textRows(), Display::textCols(), 20);
    kvt = new SimpleVT(Display::textRows(), Display::textCols());
    SB::ok();
    
    SB::progress("Initializing IDT...");
    IDT();
    SB::ok();

    SB::progress("Setting up device manager...");
    Device();
    SB::ok();
    
    SB::progress("Setting up timer...");
    Device::registerDevice(new Timer(20));
    SB::ok();
    
    SB::progress("Initializing multitasking...");
    Task("none", kvt);
    SB::ok(); 
    
    IO::sti();
    
    //============================ Device setup ================================
    SB::progress("Initializing keyboard...");
    Keyboard();
    Device::registerDevice(new PS2Keyboard());
    Keyboard::setFocus(kvt);
    SB::ok();
    
    //============================ Finish ======================================
    SB::progress("Finished!");
    SB::ok();
    
    delete sb;
    kvt->map(0, 0);
    
    //============================ Testing =====================================
    *kvt << "[Video] Resolution: " << (int)Display::textCols() << ":" << (int)Display::textRows() << "\n";
    
    while (true) {
        Vector<String> v = kvt->readLine(true).split(' ');
        //for (uint i = 0; i < v.size(); i++) {
          //  *kvt << v[i] << "\n";
       // }
        
        Print(header);
        PhysMem::getMemoryMap();
        
        //ulong* a = new ulong[0x10000];
        //*kvt << "E: " << (ulong)a << " | ";
        //Thread* t = new Thread((ThreadEntry)prog1, 0, true);
    }

    for (;;);
}











void Print(header_T* h) {
    *kvt << "|--------------------|------------|-----------------------------|\n";
    *kvt << "| Base address       | Length     | Type                        |\n";
    *kvt << "|--------------------|------------|-----------------------------|\n";
    
    for (uint i = 0; i < h->mapLen; i++) {
        *kvt << "| " << (ulong)h->memMap[i].base << " | " << (uint)h->memMap[i].length << " | ";
        
        switch (h->memMap[i].type) {
            case 1:
                *kvt << "Free memory                ";
                break;
            case 2:
                *kvt << "Reserved memory            ";
                break;
            case 3:
                *kvt << "ACPI reclaimable memory    ";
                break;
            case 4:
                *kvt << "ACPI NVS memory            ";
                break;
            case 5:
                *kvt << "Bad memory                 ";
                break;
            case 6:
                *kvt << "ISA DMA memory             ";
                break;
            case 7:
                *kvt << "Kernel memory              ";
                break;
            case 8:
                *kvt << "Initrd memory              ";
                break;
            case 9:
                *kvt << "Video backbuffer           ";
                break;
        }
        *kvt << " | \n";
    }
    
    *kvt << "|--------------------|------------|-----------------------------|\n";
}


//Test V86
    //V86Thread::regsT r;
    //Memory::clear(&r);
    //r.ax = 0x0F << 4;
    //V86::biosInt(0x12, r);
    //*kvt << "int: " << (uint)r.ax;


//Test multitaskingu
/*
 ulong syscall(ulong n, ulong a, ulong b, ulong c, ulong d, ulong e) {
    ulong r;
    asm volatile ("int $64" : "=a"(r) : "a"(n), "b"(a), "c"(b), "d"(c), "D"(d), "S"(e));
    return r;
}

void prog1() {
    ulong ret = syscall(0xFFFFFFFE00000000 | VTIF_SGETPROUTVT, VTIF_OBJTYPE, 0, 0, 0, 0);
    
    String *s = new String;
    *s = "X";
    
    for (;;) syscall(ret << 32 | VTIF_WRITE, (ulong)s, 0, 0, 0, 0);
}

void prog2() {
    ulong ret = syscall(0xFFFFFFFE00000000 | VTIF_SGETPROUTVT, VTIF_OBJTYPE, 0, 0, 0, 0);
    
    String *s = new String;
    *s = "Y";
    
    for (;;) syscall(ret << 32 | VTIF_WRITE, (ulong)s, 0, 0, 0, 0);
}
 */    
    //new Thread((ThreadEntry)prog1, 0, true);
    //new Thread((ThreadEntry)prog2, 0, true);