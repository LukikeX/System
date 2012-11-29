#ifndef WCHAR_H
#define WCHAR_H

#include <Core/typedef.h>

enum {
    UE_UTF8
};

class WChar {
private:
    union ucharReprT {
        char c[4];
        uint i;
    };
    
    static WChar CP437[];
    
public:
    uint value;
    
    WChar();
    WChar(char c);
    WChar(const char* c, uchar encoding = UE_UTF8);
    
    static uint ucharLen(const char* c, uchar encoding = UE_UTF8);
    static uint utfLen(const char* c, uchar encoding = UE_UTF8);
    
    void affectAscii(char c);
    uint affectUtf8(const char* c);
    
    uint affect(const char* c, uchar encoding = UE_UTF8) {
        if (encoding == UE_UTF8)
            return affectUtf8(c);
        
        affectAscii(c[0]);
        return 1;
    }
    
    uchar toAscii();
    ucharReprT toUtf8();
    
    ucharReprT encode(uchar encoding = UE_UTF8) {
        if (encoding == UE_UTF8) return toUtf8();
        
        ucharReprT x;
        x.c[0] = toAscii();
        return x;
    }
    
    inline WChar operator+ (uint other) {
        WChar ret;
        ret.value = value + other;
        return ret;
    }
    
    inline WChar operator- (uint other) {
        WChar ret;
        ret.value = value - other;
        return ret;
    }
    
    inline WChar& operator += (uint other) {
        value += other;
        return *this;
    }
    
    inline WChar& operator -= (uint other) {
        value -= other;
        return *this;
    }
    
    inline bool operator== (uint other) {
        return value == other;
    }
    
    inline uint operator= (uint v) {
        value = v;
        return v;
    }
    
    inline operator uint () const { return value; }
};

#endif