#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <VTManager/SimpleVT.h>

class SB {
private:
    static SimpleVT* prog, *debug;
    static uint count, inProg, length;
    
public:
    SB(uint count);
    static void progress(const String& str);
    static void ok();
    static void failed();
};

#endif