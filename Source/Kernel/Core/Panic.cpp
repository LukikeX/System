#include "Panic.h"
#include "IO.h"
#include "TaskManager/V86/V86.h"
#include <VTManager/SimpleVT.h>
#include <DeviceManager/Display.h>

void panic(const String&) {
    IO::cli();
    SimpleVT* vt = new SimpleVT(Display::textRows(), Display::textCols());
    vt->map(0, 0);
    
    vt->setColor(4);
    *vt << "\n\n";
    *vt << "                         ..-^~~~^-..\n";
    *vt << "                       .~           ~.\n";
    *vt << "                      (;:   BOOM!   :;)\n";
    *vt << "  ________             (:           :)\n";
    *vt << " | PANIC! |              ':._   _.:'\n";
    *vt << " |_  _____|                  | |\n";
    *vt << "   |/                      (=====)\n";
    *vt << " _0_                         | |\n";
    *vt << "  |                          | |\n";
    *vt << " / \\                      ((/   \\))\n";
    
    vt->setColor(2);
    *vt << "----------------------------------------------------\n";
    
    vt->setColor(3);
    *vt << "           Unhandled exception in Kernel!\n\n";
    
    vt->setColor(7);
    *vt << "   Dumping registers:\n";
    
//==============================================================================
    IDT::regs r;
    
    asm volatile ("mov %%rax, %0" : "=r" (r.rax));
    asm volatile ("mov %%rbx, %0" : "=r" (r.rbx));
    asm volatile ("mov %%rcx, %0" : "=r" (r.rcx));
    asm volatile ("mov %%rdx, %0" : "=r" (r.rdx));
    
    asm volatile ("mov %%rsp, %0" : "=r" (r.rsp));
    asm volatile ("mov %%rbp, %0" : "=r" (r.rbp));
    asm volatile ("mov %%rsi, %0" : "=r" (r.rsi));
    asm volatile ("mov %%rdi, %0" : "=r" (r.rdi));
    
    asm volatile ("mov %%cs, %0" : "=r" (r.cs));
    asm volatile ("mov %%ss, %0" : "=r" (r.ss));
    
    asm volatile ("pop %0" : "=r" (r.rip));
    asm volatile ("pushf\n pop %0" : "=r" (r.flags));
    
    r.intNo = 0x1000;
    r.errorCode = 0;
    
    dumpRegisters(&r, vt);
    for (;;);
}

void dumpRegisters(IDT::regs* r, VirtualTerminal* vt) {
    // intNo
    *vt << " [INT: ";
    vt->setColor(3);
    *vt << r->intNo;
    vt->setColor(7);
    *vt << " ] ";
    
    // Error Code
    *vt << " [ERR: ";
    vt->setColor(3);
    *vt << r->errorCode;
    vt->setColor(7);
    *vt << " ] \n";
    
//==============================================================================
    // RAX
    *vt << " [RAX: ";
    vt->setColor(3);
    *vt << r->rax;
    vt->setColor(7);
    *vt << " ] ";
    
    // RBX
    *vt << " [RBX: ";
    vt->setColor(3);
    *vt << r->rbx;
    vt->setColor(7);
    *vt << " ] \n";
    
    // RCX
    *vt << " [RCX: ";
    vt->setColor(3);
    *vt << r->rcx;
    vt->setColor(7);
    *vt << " ] ";
    
    // RDX
    *vt << " [RDX: ";
    vt->setColor(3);
    *vt << r->rdx;
    vt->setColor(7);
    *vt << " ] \n";
    
//==============================================================================
    // RSP
    *vt << " [RSP: ";
    vt->setColor(3);
    *vt << r->rsp;
    vt->setColor(7);
    *vt << " ] ";
    
    // RBP
    *vt << " [RBP: ";
    vt->setColor(3);
    *vt << r->rbp;
    vt->setColor(7);
    *vt << " ] \n";
    
    // RSI
    *vt << " [RSI: ";
    vt->setColor(3);
    *vt << r->rsi;
    vt->setColor(7);
    *vt << " ] ";
    
    // RDI
    *vt << " [RDI: ";
    vt->setColor(3);
    *vt << r->rdi;
    vt->setColor(7);
    *vt << " ] \n";
    
//==============================================================================
    ushort regX;
    
    // CS
    *vt << " [CS: ";
    vt->setColor(3);
    *vt << (ushort)r->cs;
    vt->setColor(7);
    *vt << " ] ";
    
    // DS
    *vt << " [DS: ";
    vt->setColor(3);
    asm volatile ("mov %%ds, %0" : "=r" (regX));
    *vt << regX;
    vt->setColor(7);
    *vt << " ] ";
    
    // SS
    *vt << " [SS: ";
    vt->setColor(3);
    *vt << (ushort)r->ss;;
    vt->setColor(7);
    *vt << " ] \n";
    
    // ES
    *vt << " [ES: ";
    vt->setColor(3);
    asm volatile ("mov %%es, %0" : "=r" (regX));
    *vt << regX;
    vt->setColor(7);
    *vt << " ] ";
    
    // FS
    *vt << " [FS: ";
    vt->setColor(3);
    asm volatile ("mov %%fs, %0" : "=r" (regX));
    *vt << regX;
    vt->setColor(7);
    *vt << " ] ";
    
    // GS
    *vt << " [GS: ";
    vt->setColor(3);
    asm volatile ("mov %%gs, %0" : "=r" (regX));
    *vt << regX;
    vt->setColor(7);
    *vt << " ] \n";
    
//==============================================================================
    // RFLAGS
    *vt << " [RFLAGS: ";
    vt->setColor(3);
    *vt << r->flags;
    vt->setColor(7);
    *vt << " ] \n";
    
    // RIP
    *vt << " [RIP: ";
    vt->setColor(3);
    *vt << r->rip;
    vt->setColor(7);
    *vt << " ] \n";
}