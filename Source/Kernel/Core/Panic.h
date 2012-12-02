#ifndef PANIC_H
#define PANIC_H

#include <Library/String.h>
#include "Loader.h"
#include <SyscallManager/IDT.h>

void panic(const String& str);
void dumpRegisters(IDT::regs* r, SimpleVT* vt);

#endif