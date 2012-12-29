#include "V86Thread.h"
#include "V86.h"
#include "TaskManager/Task.h"
#include <MemoryManager/Memory.h>

extern "C" void _v86_run();

V86Thread::V86Thread(functionT* entry, retvalT* ret) : Thread() {
    this->ret = ret;
    setup();
    continueOnIret = true;
    
    this->ret->regs->cs = V86::allocSeg(entry->size);
    uchar* codeptr = (uchar *)(FP_TO_LINEAR(this->ret->regs->cs, 0));
    Memory::copy(entry->data, codeptr, entry->size);
    
    this->ret->regs->ip = 0;
    state = RUNNING;
    process->registerThread(this);
    Task::registerThread(this);
}

V86Thread::V86Thread(uchar intNo, retvalT* ret) : Thread() {
    this->ret = ret;
    setup();
    continueOnIret = false;
    
    ushort* ivt = 0;
    
    this->ret->regs->cs = ivt[intNo * 2 + 1];
    this->ret->regs->ip = ivt[intNo * 2];
    
    state = RUNNING;
    process->registerThread(this);
    Task::registerThread(this);
}

void V86Thread::setup() {
    ret->finished = false;
    xchSpace = 0;
    isKernel = true;
    process = Task::currentProcess();
    m_if = true;
    kernelStack.address = Memory::alloc(STACKSIZE);
    kernelStack.size = STACKSIZE;
    
    process->getPageDir()->switchTo();
    V86::map();
    
    ret->regs->ss = V86::allocSeg(V86_STACKSIZE);
    ret->regs->sp = V86_STACKSIZE - 4;
    
    ulong* stack = (ulong *)((ulong)kernelStack.address + kernelStack.size);
    rbp = (ulong)stack;
    stack--;
    *stack = (ulong)ret->regs->cs;
    stack--;
    *stack = (ulong)ret->regs->ip;
    stack--;
    *stack = 0;
    
    rsp = (ulong)stack;
    rip = (ulong)_v86_run;
}

bool V86Thread::handleV86GPF(IDT::regs*) {
    return false;
}
#include <Core/Loader.h>
void V86Thread::handleException(IDT::regs* r) {
 //   *kvt << "hndl: " << r->intNo;
  //  for (;;);
 //   if (r->intNo == 13) {
  //      if (!handleV86GPF(r)) {
            ret->finished = true;
            ret->regs->ax = (ushort)r->rax;
            ret->regs->bx = (ushort)r->rbx;
            ret->regs->cx = (ushort)r->rcx;
            ret->regs->dx = (ushort)r->rdx;
            ret->regs->di = (ushort)r->rdi;
            ret->regs->si = (ushort)r->rsi;
            Task::currentThreadExits(0);
     //   }
    //} else
  //      Thread::handleException(r);
}