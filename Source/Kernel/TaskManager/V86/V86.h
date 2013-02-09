#ifndef V86_H
#define V86_H

#include "V86Thread.h"
#include <TaskManager/Process.h>

//#define MK_FP(seg, off) ((ulong)(((uint)seg << 16) | (ushort)off))
//#define FP_SEG(fp) (((FARPTR)fp) >> 16)
//#define FP_OFF(fp) (((FARPTR)fp) & 0xFFFF)
#define LIN_SEG(ptr) (((ulong) ptr >> 4) & 0xFFFF)
#define LIN_OFF(ptr) (((ulong) ptr) & 0xF)
#define FP_TO_LINEAR(seg, off) ((void *)(((ulong)seg << 8) + (ulong)off))
//#define FP_TO_LINEAR(seg, off) ((void*)((((u16int)(seg)) << 4) + ((u16int)(off))))

//inline uint LINEAR_TO_FP(void* ptr) {
 //   uint seg, off;
  //  off = LIN_OFF(ptr);
   // /seg = LIN_SEG(ptr);
   // return MK_FP(seg, off);/
//}

#define V86_ALLOC_START 0x1000
#define V86_ALLOC_END 0x9000

class V86 {
private:
public:
    static ushort seg;
    
public:
    static void run(V86Thread::functionT& entry, V86Thread::regsT& regs);
    static void biosInt(uchar intNo, V86Thread::regsT& regs);
    
    static void map(Process* p = 0);
    static ushort allocSeg(ushort size);
    static void* alloc(ushort size);
};

#endif