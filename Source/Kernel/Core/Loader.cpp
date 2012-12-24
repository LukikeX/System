#include "Loader.h"
#include "Devices/Timer.h"
#include "typedef.h"
#include "Panic.h"
#include "SB.h"
#include "IO.h"

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
//dorobit display dm, chyba tam select mode....
//VGATextoutput dokoncit a nahodit V86
//Virtual terminal keyboard - chyba task ...
//Definovat EOF
//Simple VT - handle escape
//ScrollableVT - redraw()....
//mutex
//String number
//Opravit bitset, nejako vynechava bity
//Dorobit PageAlloc - alokovanie novych blokov trochu blbne

//Task:
// Dorobit allocKernelPage

//Process:
//DirectoryNode* cwd
//List<File *> fileDescriptors
//syscalls
//register/unregister file descriptor - metody
//set/get cwd - metody
//exit() - file desc
//run()

//Thread:
//syscall
//


/* Syscall
 * request task switch - 64
 * signal that thread ended - 65
 * main - 66
 * debug - 67
 */

void prog1() {
    for (;;) asm ("int $64");
}

void prog2() {
    for (;;) asm ("int $65");
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
    
    new Thread((ThreadEntry)prog1, 0, true);
    new Thread((ThreadEntry)prog2, 0, true);
    
    for (;;);
}


//Keyboard();
//Device::registerDevice(new PS2Keyboard());
//Keyboard::setFocus(kvt);
   