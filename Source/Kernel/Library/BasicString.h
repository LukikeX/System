#ifndef BASICSTRING_H
#define BASICSTRING_H

#include <Core/typedef.h>
#include "Vector.h"

template<typename T>
class BasicString {
protected:
    T* string;
    uint length;
    
public:
    BasicString();
    BasicString(const T* string, uint length);
    BasicString(const BasicString<T>& other);
    BasicString(const T value, uint count = 1);
    virtual ~BasicString();
    
    void affect(const  BasicString<T>& other);
    void affect(const T* string, uint length);
    void affect(const T value, uint count = 1);
    void operator= (const BasicString<T>& other) { affect(other); }
    
    bool compare(const BasicString<T>& other) const;
    bool compare(const T* string, uint length) const;
    bool operator== (const BasicString<T>& other) const { return compare(other); }
    bool operator!= (const BasicString<T>& other) const { return !compare(other); }
    
    BasicString<T>& append(const BasicString<T>& other);
    BasicString<T>& append(const T* string, uint length);
    BasicString<T>& append(const T other);
    BasicString<T>& operator+= (const BasicString<T>& other) const { return append(other); }
    BasicString<T>& operator+= (const T other) const { return append(other); }
    
    BasicString<T> concat(const BasicString<T>& other) const;
    BasicString<T> concat(const T* string, uint length) const;
    BasicString<T> concat(const T other) const;
    
    T& operator[] (int idx) const { return string[idx]; }
    uint size() const { return length; }
    bool empty() const { return length == 0; }
    
    void clear();
    bool contains(const T& c) const;
    
    Vector<BasicString<T> > split(T sep) const;
    BasicString<T> substr(int start, uint size = -1);
    
    bool operator< (const BasicString<T>& other) const;
    bool operator> (const BasicString<T>& other) const { return (other < *this); }
};

#include "BasicString.cpp.h"

#endif