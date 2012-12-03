#include "Loader.h"
#include "Devices/Timer.h"
#include "typedef.h"
#include "Panic.h"
#include "SB.h"
#include "IO.h"
#include "DeviceManager/Time.h"
#include <C++/Runtime.h>

#include <MemoryManager/Memory.h>
#include <MemoryManager/GDT.h>

#include <DeviceManager/Device.h>
#include <DeviceManager/Display.h>

#include <Devices/Display/VGATextOutput.h>
#include <Devices/Keyboard/PS2Keyboard.h>

#include <VTManager/SimpleVT.h>
#include <VTManager/ScrollableVT.h>

#include <TaskManager/Task.h>

//dorobit irq handler
//dorobit display dm, chyba tam select mode....
//VGATextoutput dokoncit a nahodit V86
//Virtual terminal keyboard - chyba task ...
//Definovat EOF
//Simple VT - handle escape
//DOkoncit keyboard process
//ScrollableVT - redraw()....
//opravit free() v heape
//dorobit page directory

//dorobit multitasking
//v task.cpp dorobit allocKernelPage

/* Syscall
 * request task switch - 66
 * signal that thread ended - 77
 * main - 88
 * debug - 99
 */

ulong test() {
    while (true)
        asm ("int $88");
    
    return 1;
}



SimpleVT* kvt;

extern "C" void Loader() {
    Memory::placementAddress = (ulong)&_end;
    construct();
    
    VGATextoutput* vgaout = new VGATextoutput();
    Display::setText(vgaout);
    
    SB* sb = new SB(9);
    
    SB::progress("Initializing paging...");
    PhysMem();
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
    Device::registerDevice(new Timer());
    SB::ok();
    
    SB::progress("Initializing multitasking...");
    Task("none", kvt);
    SB::ok();
     
    
    SB::progress("Finished!");
    SB::ok();
    
    //for (uint i = 0; i < 10000000; i++);
    delete sb;
    kvt->map(0, 0);
    
    IO::sti();
    
    //new Thread((ThreadEntry)test, 0, true);
    
    while (true) {
        *kvt << (uint)Task::processes->size() << "\n";
        for (uint i = 0; i < 50000000; i++);
    }
    
 
    //*kvt << "lol";
    //Device::registerDevice(new PS2Keyboard());
    //panic("test");
    
    for (;;);
}