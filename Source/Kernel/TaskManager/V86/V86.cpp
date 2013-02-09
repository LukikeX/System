#include "V86.h"
#include "TaskManager/Task.h"

ushort V86::seg = 0x1000;

void V86::run(V86Thread::functionT& entry, V86Thread::regsT& regs) {
    V86Thread::retvalT ret;
    ret.regs = &regs;
    new V86Thread(&entry, &ret);
    
    while (!ret.finished)
        Task::currentThread()->sleep(10);
}

void V86::biosInt(uchar intNo, V86Thread::regsT& regs) {
    V86Thread::retvalT ret;
    ret.regs = &regs;
    new V86Thread(intNo, &ret);
    
    while (!ret.finished)
        Task::currentThread()->sleep(10);
}

void V86::map(Process* p) {
    if (!p)
        p = Task::currentProcess();
    
    for (uint i = 0; i < 0xFFFF; i += 0x1000)
        p->getPageDir()->allocFrame(i, true, true);
}

ushort V86::allocSeg(ushort size) {
    if (size & 0x0F)
        size = (size & 0xFFFF0) + 0x10;
    ushort segments = size / 16;
    
    if (seg  < 0x1000)
        seg = 0x1000;
    
    if (seg + segments > 0x9000)
        seg = 0x1000;
    
    ushort ret = seg;
    seg += segments;
    return ret;
}

void* V86::alloc(ushort size) {
    return (void *)allocSeg(size);
    //return FP_TO_LINEAR(allocSeg(size), 0);
}