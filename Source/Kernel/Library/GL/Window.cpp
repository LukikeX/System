#include "Window.h"
#include <DeviceManager/Display.h>

namespace GL {
    Window* Window::window = 0;
    
    void Window::testDraw() {
        Memory::copy(buffer, (uint *)0xFFFFFFFFF0000000, width * height);
    }
    
    Window::Window(uint width, uint height) {
        this->width = width;
        this->height = height;
        buffer = 0;
        
        loadIdentity();
    }
    
    void Window::addPrim(Prim* p) {
        removePrim(p);
        prims.push(p);
    }
    
    void Window::removePrim(Prim* p) {
        for (uint i = 0; i < prims.size(); i++) {
            if (prims[i] == p) {
                prims[i] = prims.back();
                prims.pop();
                return;
            }
        }
    }
    
    void Window::loadIdentity() {
        x = (float)width / 2;
        y = (float)height / 2;
        z = 0;
        
        if (buffer)
            delete[] buffer;
        
        buffer = new uint[width * height];
        Memory::clear(buffer, width * height);
    }
    
    void Window::translate(float x, float y, float z) {
        this->x += x;
        this->y += y;
        this->z += z;
    }
    
    void Window::update() {
        Memory::clear(buffer, width * height);
        
        for (uint i = 0; i < prims.size(); i++)
            prims[i]->draw();
    }
}