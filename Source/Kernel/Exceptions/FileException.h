#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include "Exception.h"

class FileException : public Exception {
public:
    FileException() throw() : Exception() {}
    FileException(const char* str, uint id = 0) throw() : Exception(str, id) {}
    FileException(const Exception& other) throw() : Exception(other) {}
};

#endif