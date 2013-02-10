#ifndef GL_QUAD_H
#define GL_QUAD_H

#include "Prim.h"

namespace GL {
    class Quad : public Prim {
        
    public:
        Quad();
        void draw();
    };
}

#endif