#include "Prim.h"

namespace GL {
    Prim::Prim(uint count) {
        this->count = count;
        used = 0;
        pos = new float*[3];
        
        for (uint i = 0; i < 3; i++)
            *pos = new float[count];
    }
    
    void Prim::vertex(float x, float y, float z) {
        pos[0][used] = x;
        pos[1][used] = y;
        pos[2][used] = z;
        
        if (used == count) {
            //draw();
        } else
            used++;
    }
}