#ifndef MEMORYOEXCEPTION_H
#define MEMORYOEXCEPTION_H

#include "Exception.h"

class MemoryException : public Exception {
public:
    MemoryException() throw() : Exception() {}
    MemoryException(const char* str, uint id = 0) throw() : Exception(str, id) {}
    MemoryException(const Exception& other) throw() : Exception(other) {}
};

#endif