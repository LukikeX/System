#include "Loader.h"
#include "typedef.h"
#include "Panic.h"
#include "SB.h"

#include "VTManager/SimpleVT.h"
#include "VTManager/ScrollableVT.h"
#include "DeviceManager/Device.h"
#include "Devices/Keyboard/PS2Keyboard.h"
#include "IO.h"
#include "C++/Runtime.h"

#include <MemoryManager/Memory.h>
#include <MemoryManager/GDT.h>

#include <DeviceManager/Display.h>
#include <Devices/Display/VGATextOutput.h>

#include <VTManager/SimpleVT.h>

//Panic - dokoncit
//Mutex - waitLock()
//dorobit irq handler
//dorobit display dm, chyba tam select mode....
//VGATextoutput dokoncit a nahodit V86
//Virtual terminal keyboard - chyba task ...
//Definovat EOF
//Simple VT - handle escape
//DOkoncit keyboard process
//ScrollableVT - redraw()....
//opravit free() v heape
//Memory dokoncit mkXchgSpace

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
    
    new SB(5);
    
    SB::progress("Initializing paging...");
    PhysMem();
    SB::ok();
    
    SB::progress("Initializing memory heap...");
    Memory();
    SB::ok();
    
    SB::progress("Initializing Global Description Table...");
    GDT();
    SB::ok();
    
    SB::progress("Creating kernel virtual terminal...");
    kvt = new ScrollableVT(Display::textRows(), Display::textCols(), 20);
    SB::ok();
    
    SB::progress("Initializing Interrupt Description Table...");
    IDT();
    SB::ok();
    
   
    IO::sti();
    
    
    //kvt->map(0, 0);
    
    //IO::cli();
    //new IDT();
    //Device::registerDevice(new PS2Keyboard());
    //IO::sti();
    
  //  kvt->getkeyPress(true, false);
  //  kvt->getkeyPress(true, false);
   // kvt->getkeyPress(true, false);
  //  kvt->getkeyPress(true, false);
    //*kvt << "test";
    
    //for (uint i = 1; i; i++) {
        //*kvt << "test: " << (int)i << "\n";
        //for (uint j = 0; j < 1000000; j++);
    //}
    //*kvt << "test\n";
    
   // panic("test");
    
    
    for (;;);
}