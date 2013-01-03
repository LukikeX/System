#include "Loader.h"
#include "Devices/Timer.h"
#include "typedef.h"
#include "Panic.h"
#include "SB.h"
#include "IO.h"
#include "Exceptions/MemoryException.h"

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
//odkomentovat v process a atacontroller kod ked pojde VFS
//dorobit mount vo VFS

//Process:
//run()
//scall - uid
//Dorobit par detailov v syscall

/* Syscalls port
 * syscall - 64
 * request task switch - 65
 * signal that thread ended - 66
 */

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



SimpleVT* kvt;

extern "C" void Loader() {
    Memory::placementAddress = (ulong)&_end;
    construct();
    
    VGATextoutput* vgaout = new VGATextoutput();
    Display::setText(vgaout);
    
    SB* sb = new SB(10);    
    SB::progress("Initializing paging...");
    PhysMem();
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
    
    SB::progress("Finished!");
    SB::ok();
    
    delete sb;
    kvt->map(0, 0);
    IO::sti();
    
    //new Thread((ThreadEntry)prog1, 0, true);
  //  new Thread((ThreadEntry)prog2, 0, true);
    
    //V86Thread::regsT r;
    //Memory::clear(&r);
    //r.ax = 0x0F << 4;
    //V86::biosInt(0x12, r);
    //*kvt << "int: " << (uint)r.ax;

    *kvt << "pol";
    
    for (;;);
}


//Keyboard();
//Device::registerDevice(new PS2Keyboard());
//Keyboard::setFocus(kvt);
   