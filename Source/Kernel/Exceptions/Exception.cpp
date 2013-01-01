#include "Exception.h"

Exception::Exception() {
    message = "Undefined exception";
    id = 0;
}

Exception::Exception(String& str, uint id) {
    message = String(str);
    this->id = id;
}

Exception::Exception(const Exception& other) {
    message = other.message;
    id = other.id;
}

Exception& Exception::operator =(const Exception& other) {
    message = other.message;
    id = other.id;
}