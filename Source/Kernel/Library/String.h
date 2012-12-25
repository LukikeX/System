#ifndef STRING_H
#define STRING_H

#include "BasicString.h"
#include "WChar.h"

class String : public BasicString<WChar> {
public:
    String() : BasicString<WChar>() { }
    String(WChar c, uint n = 1) : BasicString<WChar>(c, n) { }
    String(const String& other) : BasicString<WChar>(other) { }
    String(const char* string, uchar encoding = UE_UTF8);
    
    void affect(const char* string, uchar encoding = UE_UTF8);
    void operator= (const char* other) { affect(other); }
    void operator= (const String& other) { BasicString<WChar>::affect(other); }
    
    bool compare(const char* string, uchar encoding = UE_UTF8) const;
    bool operator== (const char* other) const { return compare(other); }
    bool operator!= (const char* other) const { return !compare(other); }
    bool operator== (const String& other) const { return BasicString<WChar>::compare(other); }
    bool operator!= (const String& other) const { return !BasicString<WChar>::compare(other); }
    
    String& append(const char* other, uchar encoding = UE_UTF8);
    String& operator+= (const String& other) { BasicString<WChar>::append(other); return *this; }
    String& operator+= (const char* other) { return append(other); }
    String& operator+= (WChar other) { BasicString<WChar>::append(other); return *this; }
    
    String concat(const String& other) const;
    String concat(const char* other, uchar encoding = UE_UTF8) const;
    String concat(WChar other) const;
    String operator+ (const String& other) const { return concat(other); }
    String operator+ (const char* other) const { return concat(other); }
    String operator+ (WChar other) const { return concat(other); }
    
    long toInt() const;
    ulong toInt16() const;
    uint getLength() const { return length; }
    
    Vector<String> split(WChar c) const;
    String substr(int start, int size = -1);
    ulong serialize() const;
    static String unserialize(ulong w);
    
    static uint strlen(const char* str);
    static String hex(uint n);
    static String hex(ulong n);
    static String number(long n);
};

#endif