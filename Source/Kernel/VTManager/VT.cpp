#include "VT.h"
#include <DeviceManager/Display.h>

Vector<SimpleVT *> VT::mappedVTs;

void VT::map(SimpleVT* vt) {
    unmap(vt);
    mappedVTs.push(vt);
}

void VT::unmap(SimpleVT* vt) {
    for (uint i = 0; i < mappedVTs.size(); i++) {
        if (mappedVTs[i] == vt) {
            mappedVTs[i] = mappedVTs.back();
            mappedVTs.pop();
            redrawScreen();
            return;
        }
    }
}

void VT::redrawScreen() {
    Display::clear();
    
    for (uint i = 0; i < mappedVTs.size(); i++)
        mappedVTs[i]->redraw();
}