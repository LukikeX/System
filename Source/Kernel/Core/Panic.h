#ifndef PANIC_H
#define PANIC_H

#include <Library/String.h>
#include "Loader.h"
#include <SyscallManager/IDT.h>

void panic(const String& str, IDT::regs *rgs = 0);
void dumpRegisters(IDT::regs* r, VirtualTerminal* vt);

#endif