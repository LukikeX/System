#include "V86Thread.h"
#include "V86.h"
#include <MemoryManager/Memory.h>

V86Thread::V86Thread(functionT* entry, retvalT* ret) {
    this->ret = ret;
    setup();
    continueOnIret = true;
    
    this->ret->regs->cs = V86::allocSeg(entry->size);
//    uchar* codeptr = (uchar *)(FP_TO_LINEAR(this->ret->regs->cs, 0));
 //   Memory::copy(entry->data, codeptr, entry->size);
    
    this->ret->regs->ip = 0;
    //state = t
}

void V86Thread::setup() {
    
}