#include "Loader.h"
#include "typedef.h"
#include "VTManager/SimpleVT.h"
#include "VTManager/ScrollableVT.h"

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

/* Syscall
 * request task switch - 66
 * signal that thread ended - 77
 * main - 88
 * debug - 99
 */

extern "C" void Loader() {
    Memory::placementAddress = (ulong)&_end;
    
    PhysMem();
    Memory();
    GDT();
    
    VGATextoutput* vgaout = new VGATextoutput();
    Display::setText(vgaout);
    
    SimpleVT* kvt;
    kvt = new ScrollableVT(20, 20, 20);
    kvt->map(0, 0);
    *kvt << "test\n";
    *kvt << "test\n";
    for (;;);
}