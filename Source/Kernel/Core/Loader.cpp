#include "Loader.h"
#include "typedef.h"
#include "Panic.h"
#include "SB.h"
#include "IO.h"
#include <Shell/KernelShell.h>

#include <C++/Runtime.h>
#include <VTManager/VT.h>
#include <VTManager/ScrollableVT.h>
#include <VFS/VFS.h>

#include <MemoryManager/PageAlloc.h>
#include <MemoryManager/Memory.h>
#include <MemoryManager/GDT.h>

#include <DeviceManager/Device.h>
#include <DeviceManager/Display.h>
#include <DeviceManager/Time.h>
#include <DeviceManager/Keyboard.h>
#include <DeviceManager/PCI.h>

#include <Devices/Display/VGATextOutput.h>
#include <Devices/Keyboard/PS2Keyboard.h>
#include <Devices/Display/VESADisplay.h>
#include <Devices/Mouse/PS2Mouse.h>
#include <Devices/FPU.h>
#include <Devices/Timer.h>
#include <Devices/Network/ne2000/ne2000.h>

#include <SyscallManager/Res.h>
#include <TaskManager/Task.h>
#include <TaskManager/V86/V86.h>
#include <TaskManager/V86/V86Thread.h>

#include <NetworkManager/Net.h>

#include <Library/GL/Window.h>
#include <Library/GL/Quad.h>

//dorobit mount vo VFS
//dorobit DMA na pracu s grafikou VSESA scrollovanie...
//Opravit V86 - nejde priamo v long mode
//Dorobit Vlastny filesystem
//Dorobit usermanager

SimpleVT* kvt;
void Print(header_T* h);

extern "C" void Loader(header_T* header) {
    Memory::placementAddress = (ulong)&_end;
    construct();
    Res::size = 0;
    VFS::rootNode = 0;
    
    VGATextoutput* vgaout = new VGATextoutput();
    Display::setText(vgaout);
    
    SB* sb = new SB(15);
    SB::progress("Initializing paging...");
    ulong size = 0x100000;
    for (uint i = 0; i < header->mapLen; i++)
        if (header->memMap->base == size)
            size += header->memMap->length - 0x1000;
    
    PhysMem((ulong)size);
    SB::ok();
    
    SB::progress("Initializing free pages...");
    PageAlloc();
    SB::ok();

    SB::progress("Initializing memory heap...");
    Memory();
    SB::ok();
    
    SB::progress("Initializing VESA mode...");
    VESADisplay* vesaout = new VESADisplay();
    vesaout->currMode = *header->vesaMode;
    vesaout->getModes(Display::modes);
    Display::mode = Display::modes[0];
        
    vesaout->fb = (uchar *)0xFFFFFFFFF0000000;
    for (uint i = 0; i < (uint)(vesaout->currMode.Yres * vesaout->currMode.pitch); i += 0x1000)
        PhysMem::kernelPageDirectory->map(PhysMem::kernelPageDirectory->getPage((ulong)(vesaout->fb + i), true), (vesaout->currMode.physbase + i), false, false);
    
    vesaout->pixWidth = (vesaout->currMode.bpp + 1) / 8;
    VT::redrawScreen();
    SB::ok();
    
    SB::progress("Initializing GDT...");
    GDT();
    SB::ok();

    SB::progress("Creating Kernel VT...");
    kvt = new ScrollableVT(Display::textRows(), Display::textCols(), 20);
    SB::ok();
    
    SB::progress("Initializing IDT...");
    IDT();
    SB::ok();

    SB::progress("Setting up device manager...");
    Device();
    SB::ok();
    
    SB::progress("Enabling Float Point Unit...");
    FPU();
    SB::ok();
    
    SB::progress("Setting up timer...");
    Device::registerDevice(new Timer(100));
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
    
    SB::progress("Initializing mouse...");
    Device::registerDevice(new PS2Mouse());
    SB::ok();
    
    SB::progress("Initializing PCI cards...");
    PCI();
    SB::ok();
    
    //============================ Finish ======================================
    SB::progress("Finished!");
    SB::ok();
    
    delete sb;
    kvt->map();
    
    //============================ Testing =====================================
    VFS::mount((DirectoryNode* )0, 0x100000);
    
    DirectoryNode* cwd = VFS::getRootNode();
    Task::currentProcess()->setCwd(cwd);
    
    new KernelShell(cwd, kvt);
    while (KernelShell::getInstances())
        Task::currentThread()->sleep(100);
    
    
    //Net::ifconfig_t conf;
    //conf.ipaddr = inetAddress(192, 168, 0, 100);
    //conf.netmask = inetAddress(255, 255, 255, 0);
    //conf.broadcast = inetAddress(192, 168, 0, 255);
    //conf.mtu = 0;
    
    //NE2000* ne = new NE2000();
    //ne->readPacket();
    
    //GL::Window* w = new GL::Window(100, 100);
   // GL::Window::window = w;
    
    //GL::Quad* stvorec = new GL::Quad();
    //stvorec->vertex(-1.0f, 1.0f, 0.0f);
    //stvorec->vertex(1.0f, 1.0f, 0.0f);
    //stvorec->vertex(-1.0f, -1.0f, 0.0f);
    //stvorec->vertex(1.0f, -1.0f, 0.0f);
    
   // w->testDraw();
    
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