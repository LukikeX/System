#ifndef VT_H
#define VT_H

#include <Library/Vector.h>

#include "SimpleVT.h"

class VT {
private:
    static Vector<SimpleVT *> mappedVTs;
    
public:
    static void map(SimpleVT* vt);
    static void unmap(SimpleVT* vt);
    static void redrawScreen();
};

#endif