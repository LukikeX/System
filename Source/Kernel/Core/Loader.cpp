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

//Opravit chybu v VTcku, po sti sa mu nejako nechce makat...

/* Syscall
 * request task switch - 66
 * signal that thread ended - 77
 * main - 88
 * debug - 99
 */

SimpleVT* kvt;

extern "C" void Loader() {
    Memory::placementAddress = (ulong)&_end;
    construct();
    
    VGATextoutput* vgaout = new VGATextoutput();
    Display::setText(vgaout);
    
    SB* sb = new SB(8);
    
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
    
    SB::progress("Setting up timer...");
    Device::registerDevice(new Timer());
    IO::sti();
    SB::ok();
    
    SB::progress("Initializing multitasking...");
    Task("none", kvt);
    SB::ok();
     
    
    SB::progress("Finished!");
    SB::ok();
    
    //delete sb;
    //kvt->map(0, 0);
    
    IO::sti();
    
    //panic("test");
    
    for (;;);
}