#include "IDT.h"
#include "Core/Panic.h"
#include "DeviceManager/Time.h"
#include <Core/IO.h>
#include <DeviceManager/Device.h>
#include <TaskManager/Task.h>

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

extern "C" void int64();
extern "C" void int65();
extern "C" void int66();
extern "C" void int67();

extern "C" void interrupt_handler(IDT::regs* r) {
    IDT::handler(r);
}

IDT::entryT* IDT::entries;
IDT::ptrT* IDT::ptr;

IDT::IDT() {
    IO::outB(0x21, (uchar)0xFD);
    IO::outB(0xA1, (uchar)0xFF);
    
    //Remap
    IO::outB(0x20, 0x11);
    IO::outB(0xA0, 0x11);
    IO::outB(0x21, 0x20);
    IO::outB(0xA1, 0x28);
    IO::outB(0x21, 0x04);
    IO::outB(0xA1, 0x02);
    IO::outB(0x21, 0x01);
    IO::outB(0xA1, 0x01);
    IO::outB(0x21, 0x0);
    IO::outB(0xA1, 0x0);
    
    ptr = new ptrT;
    entries = new entryT[256];
    Memory::clear(entries, 256);
    
    ptr->limit = sizeof(entryT) * 256 - 1;
    ptr->base = (ulong)entries;
    
    setGate(0,  (ulong)isr0,  0x08, 0x8E);
    setGate(1,  (ulong)isr1,  0x08, 0x8E);
    setGate(2,  (ulong)isr2,  0x08, 0x8E);
    setGate(3,  (ulong)isr3,  0x08, 0x8E);
    setGate(4,  (ulong)isr4,  0x08, 0x8E);
    setGate(5,  (ulong)isr5,  0x08, 0x8E);
    setGate(6,  (ulong)isr6,  0x08, 0x8E);
    setGate(7,  (ulong)isr7,  0x08, 0x8E);
    setGate(8,  (ulong)isr8,  0x08, 0x8E);
    setGate(9,  (ulong)isr9,  0x08, 0x8E);
    setGate(10, (ulong)isr10, 0x08, 0x8E);
    setGate(11, (ulong)isr11, 0x08, 0x8E);
    setGate(12, (ulong)isr12, 0x08, 0x8E);
    setGate(13, (ulong)isr13, 0x08, 0x8E);
    setGate(14, (ulong)isr14, 0x08, 0x8E);
    setGate(15, (ulong)isr15, 0x08, 0x8E);
    setGate(16, (ulong)isr16, 0x08, 0x8E);
    setGate(17, (ulong)isr17, 0x08, 0x8E);
    setGate(18, (ulong)isr18, 0x08, 0x8E);
    setGate(19, (ulong)isr19, 0x08, 0x8E);
    setGate(20, (ulong)isr20, 0x08, 0x8E);
    setGate(21, (ulong)isr21, 0x08, 0x8E);
    setGate(22, (ulong)isr22, 0x08, 0x8E);
    setGate(23, (ulong)isr23, 0x08, 0x8E);
    setGate(24, (ulong)isr24, 0x08, 0x8E);
    setGate(25, (ulong)isr25, 0x08, 0x8E);
    setGate(26, (ulong)isr26, 0x08, 0x8E);
    setGate(27, (ulong)isr27, 0x08, 0x8E);
    setGate(28, (ulong)isr28, 0x08, 0x8E);
    setGate(29, (ulong)isr29, 0x08, 0x8E);
    setGate(30, (ulong)isr30, 0x08, 0x8E);
    setGate(31, (ulong)isr31, 0x08, 0x8E);
    
    setGate(32, (ulong)irq0,  0x08, 0x8E);
    setGate(33, (ulong)irq1,  0x08, 0x8E);
    setGate(34, (ulong)irq2,  0x08, 0x8E);
    setGate(35, (ulong)irq3,  0x08, 0x8E);
    setGate(36, (ulong)irq4,  0x08, 0x8E);
    setGate(37, (ulong)irq5,  0x08, 0x8E);
    setGate(38, (ulong)irq6,  0x08, 0x8E);
    setGate(39, (ulong)irq7,  0x08, 0x8E);
    setGate(40, (ulong)irq8,  0x08, 0x8E);
    setGate(41, (ulong)irq9,  0x08, 0x8E);
    setGate(42, (ulong)irq10, 0x08, 0x8E);
    setGate(43, (ulong)irq11, 0x08, 0x8E);
    setGate(44, (ulong)irq12, 0x08, 0x8E);
    setGate(45, (ulong)irq13, 0x08, 0x8E);
    setGate(46, (ulong)irq14, 0x08, 0x8E);
    setGate(47, (ulong)irq15, 0x08, 0x8E);
    
    setGate(66, (ulong)int64, 0x08, 0x8E);
    setGate(77, (ulong)int65, 0x08, 0x8E);
    setGate(88, (ulong)int66, 0x08, 0x8E);
    setGate(88, (ulong)int67, 0x08, 0x8E);
    
    asm volatile ("lidt (%0)" : : "r"((ulong)ptr));
}

void IDT::setGate(uint num, ulong base, ushort sel, uint flags) {
    entries[num].baseLow    = base & 0xFFFF;
    entries[num].baseMiddle = (base >> 16) & 0xFFFF;
    entries[num].baseHigh   = base >> 32;
    entries[num].sel        = sel;
    entries[num].flags      = flags;
    entries[num].always0    = 0;
    entries[num].always02   = 0;
}

void IDT::handler(regs* r) {
    bool doSwitch = r->intNo == 32 || r->intNo >= 66;
    
    if (r->intNo < 32) {
        if ((ulong)Task::currentThread() == 0xFFFFFFFFFFFFFFFF || !Task::currentThread())
            panic("Exception cannot be handled!");
        
       // Task::currentThread()->handleException(r);
    } else if (r->intNo < 48) {
        if (r->intNo >= 40)
            IO::outB(0xA0, 0x20);
        IO::outB(0x20, 0x20);
    
        IO::sti();
        Device::handler(r);
        IO::cli();
        
        doSwitch = doSwitch || Task::IRQwakeup(r->intNo - 32);
    } else if (r->intNo == 64) {
        IO::sti();
        
        uint res = r->rax >> 32;
        //uint wat = r->rax & 0xFFFFFFFF;
        
        if (res == 0xFFFFFFFF) {
            //test
        } else {
            //res call...
        }
        
        Task::currentProcess()->getPageDir()->switchTo();
        IO::cli();
    } else if (r->intNo == 66) {
        Task::currentThreadExits(r->rax);
    }
    
   // kvt->put('.');
    
    if (r->intNo == 14) {
        ulong fAddress;
        asm volatile ("mov %%cr2, %0" : "=r" (fAddress));
    
        String str;
        str = "\n\n";
        str += (r->errorCode & 0x1 ? "" : "Present");
        str += (r->errorCode & 0x2 ? " Read only" : "");
        str += (r->errorCode & 0x4 ? " User mode" : "");
        str += (r->errorCode & 0x8 ? " Reserved" : "");
        str += " @ ";
        str += String::hex(fAddress);
        
        *kvt << str;
        IO::cli();
        for (;;);
    }
    
    if (doSwitch)
        Task::doSwitch();
}