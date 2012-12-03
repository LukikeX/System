#include "String.h"

String::String(const char* string, uchar encoding) {
    this->string = 0;
    length = 0;
    affect(string, encoding);
}

void String::affect(const char* string, uchar encoding) {
    length = WChar::utfLen(string, encoding);

    if (this->string)
        delete[] this->string;
    
    if(!length) {
        this->string = 0;
        return;
    }
    
    this->string = new WChar[length + 1];
    uint i = 0, c = 0;
    while (c < length) {
        i += this->string[c].affect(string + i, encoding);
        c++;
    }
    this->string[length] = 0;
}

bool String::compare(const char* string, uchar encoding) const {
    if (length != WChar::utfLen(string, encoding))
        return false;
    
    int i = 0, l = strlen(string), c = 0;
    WChar tmp;
    while (i < l) {
        i += tmp.affect(string + i, encoding);
        if (this->string[c] != tmp) 
            return false;
        c++;
    }
    return true;
}

String& String::append(const char* other, uchar encoding) {
    WChar* newData = new WChar[length + WChar::utfLen(other, encoding) + 1];
    Memory::copy(string, newData, length);
    
    int i = 0, l = strlen(other), c = 0;
    while (i < l) {
        i += newData[c + length].affect(other + i, encoding);
        c++;
    }
    
    if (string)
        delete[] string;
    string = newData;
    length += strlen(other);
    string[length] = 0;
    return *this;
}

String String::concat(const String& other) const {
    String ret(*this);
    return (ret += other);
}

String String::concat(const char* other, uchar encoding) const {
    String ret(*this);
    return (ret.append(other, encoding));
}

String String::concat(WChar other) const {
    String ret(*this);
    return (ret += other);
}

long String::toInt() const {
    if (!string)
        return 0;
    
    int pos = 0;
    long number = 0;
    bool negative = false;
    
    if (string[0].value == '-') {
        negative = true;
        pos = 1;
    }
    
    while (string[pos] >= '0' && string[pos] <= '9') {
        number *= 10;
        number += (string[pos].value - '0');
        pos++;
    }
    
    if (negative)
        return 0 - number;
    return number;
}

ulong String::toInt16() const {
    if (!string)
        return 0;
    
    int pos = 0;
    long number = 0;
    
    if (string[0].value == '0' && string[1].value == 'x')
        pos = 2;
    
    while (true) {
        char c = string[pos];
        pos++;
        
        if (c >= 'a' && c <= 'f')
            c -= 'a' - 'A';
        
        if (c >= '0' && c <= '9') {
            number *= 16;
            number += c - '0';
            continue;
        }
        
        if (c >= 'A' && c <= 'F') {
            number *= 16;
            number += c - 'A' + 10;
            continue;
        }
        break;
    }
    
    return number;
}

Vector<String> String::split(WChar c) const {
    Vector<String> ret;
    ret.push(String(""));
    
    for (uint i = 0; i < length; i++) {
        if (string[i] == c)
            ret.push(String(""));
        else
            ret.back() += string[i];
    }
    return ret;
}

String String::substr(int start, int size) {
    if (!size)
        return String();
    
    if (start < 0)
        start = length - start;
    
    if (size == -1)
        size = length - size;
    
    if (start + size >= (int)length)
        size = length - start;
    
    String ret;
    ret.string = new WChar[size + 1];
    ret.length = size;
    
    Memory::copy((uchar *)ret.string, (uchar *)(string + start), size * sizeof(WChar));
    ret.string[size] = 0;
    return ret;
}


uint String::strlen(const char* str) {
    uint ret = 0;
    while(str[ret])
        ret++;
    return ret;
}

String String::hex(uint n) {
    String ret;
    ret.length = 10;
    ret.string = new WChar[11];
    ret.string[0] = '0';
    ret.string[1] = 'x';
    ret.string[10] = '0';
    
    char hexDigits[] = "0123456789ABCDEF";
    
    for (uint i = 2; i < 10; i++) {
        ret.string[i] = hexDigits[(n & 0xF0000000) >> 28];
        n = n << 4;
    }
    return ret;
}

String String::number(long n) {
    //if (!n) return String("0");
    
    bool negative = false;
    if (n < 0) {
        negative = true;
        n = 0 - n;
    }
    uint order = 0, temp = n;
    char numbers[] = "0123456789";
    while (temp > 0) {
        order++;
        temp /= 10;
    }
    
    String ret;
    ret.length = order;
    ret.string = new WChar[order + 1];
    
    for (uint i = order; i > 0; i--) {
        ret.string[i - 1] = numbers[n % 10];
        n /= 10;
    }
    
    ret.string[order] = 0;
    
   // if (negative)
     //   return String("-") += ret;
    return ret;
}

String String::hex(ulong n) {
    String ret;
    ret.length = 18;
    ret.string = new WChar[19];
    ret.string[0] = '0';
    ret.string[1] = 'x';
    ret.string[18] = '0';
    
    char hexDigits[] = "0123456789ABCDEF";
    
    for (uint i = 2; i < 18; i++) {
        ret.string[i] = hexDigits[(n & 0xF000000000000000) >> 60];
        n = n << 4;
    }
    return ret;
}