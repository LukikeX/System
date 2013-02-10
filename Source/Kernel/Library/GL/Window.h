#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <Core/typedef.h>
#include <Library/Vector.h>
#include "Prim.h"

namespace GL {
    class Window {
        friend class Prim;
    private:
        uint width, height;
        Vector<Prim *> prims;
        uint* buffer;
        float x, y, z;
        
        void addPrim(Prim* p);
        void removePrim(Prim* p);
        
    public:
        static Window* window;
        
        Window(uint width, uint height);
        void loadIdentity();
        void translate(float x, float y, float z);
        void update();
        
        void testDraw();
    };
}

#endif