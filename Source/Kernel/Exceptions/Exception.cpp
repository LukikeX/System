#include "Exception.h"

Exception::Exception() throw() {
    message = "Undefined exception";
    id = 0;
}

Exception::Exception(const char* str, uint id) throw() {
    message = String(str);
    this->id = id;
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