#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <Library/String.h>

class Exception {
private:
    String message;
    uint id;
    
public:
    Exception() throw();
    Exception(const char* str, uint id = 0) throw();
    Exception(const Exception& other) throw();
    Exception& operator=(const Exception& other) throw();
    virtual ~Exception() throw() { }
    
};

#endif