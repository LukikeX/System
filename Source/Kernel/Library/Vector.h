#ifndef VECTOR_H
#define VECTOR_H

#include <Core/typedef.h>
#include <MemoryManager/Memory.h>

template<typename T>
class Vector {
private:
    T* data;
    uint m_size;
    
public:
    Vector();
    Vector(uint size);
    Vector(uint size, const T& value);
    Vector(const Vector<T>& other);
    ~Vector();
    
    Vector<T>& operator= (const Vector<T>& other);
    T& operator[] (uint idx) const;
    
    void push(const T& element);
    void pop();
    
    T& back() const;
    T& front() const;
    
    uint size() const;
    bool empty() const;
    void clear();
};

#include "Vector.cpp.h"

#endif