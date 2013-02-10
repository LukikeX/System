#ifndef FPU_H
#define FPU_H

#include <Core/typedef.h>

class FPU {
public:
    FPU();
    void setCW(const ushort cw);
};

#endif