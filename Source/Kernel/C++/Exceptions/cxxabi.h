#ifndef CXXABI_H
#define CXXABI_H

#include "Unwind.h"
#include "typeinfo.h"

namespace __cxxabiv1 {
    extern "C" {
        typedef void (*unexpected_handler)();
        typedef void (*terminate_handler)();
        
        struct __cxa_exception {
            ulong referenceCount;
            std::type_info* exceptionType;
            void (*exceptionDestructor)(void *);
            unexpected_handler unexpectedHandler;
            terminate_handler terminateHandler;
            
            __cxa_exception* nextException;
            int handlerCount;
            
            int handlerSwitchValue;
            const char* actionRecord;
            const char* languageSpecificData;
            void* catchTemp;
            void* adjustedPtr;
            
            _Unwind_Exception unwindHandler;
        };
        
        struct __cxa_eh_globals {
            __cxa_exception* caughtExceptions;
            ulong uncaughtExceptions;
        };
        
        struct __cxa_thread_info {
            terminate_handler terminateHandler;
            unexpected_handler unexpectedHandler;
    
            int emergencyBuffersHeld;
            _Unwind_Exception* currentCleanup;
            __cxa_eh_globals globals;
        };
    }
}

#endif