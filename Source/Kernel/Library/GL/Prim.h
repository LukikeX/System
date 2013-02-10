#ifndef GL_PRIM_H
#define GL_PRIM_H

#include <Core/typedef.h>

namespace GL {
    class Window; 
    
    class Prim {
        friend class Window;
        
    protected:
        uint count, used;
        float** pos;
        
        Prim(uint count);
        virtual void draw() = 0;
        
    public:
        void vertex(float x, float y, float z);
    };
}

#endif