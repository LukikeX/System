#include "Exception.h"
#include <Core/IO.h>
#include <Core/Loader.h>

Exception::Exception() throw() {
    message = "Undefined exception";
    id = 0;
}

Exception::Exception(const char* str, uint id) throw() {
    message = String(str);
    this->id = id;
    
    IO::cli();
    *kvt << str;
    for (;;);
}

Exception::Exception(const Exception& other) throw() {
    message = other.message;
    id = other.id;
}

Exception& Exception::operator =(const Exception& other) throw() {
    message = other.message;
    id = other.id;
    return *this;
}