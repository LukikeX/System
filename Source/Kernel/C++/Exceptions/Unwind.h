#ifndef UNWIND_H
#define UNWIND_H

#include <Core/typedef.h>

extern "C" {
    typedef enum {
        _URC_NO_REASON = 0,
        _URC_FOREIGN_EXCEPTION_CAUGHT = 1,
        _URC_FATAL_PHASE2_ERROR = 2,
        _URC_FATAL_PHASE1_ERROR = 3,
        _URC_NORMAL_STOP = 4,
        _URC_END_OF_STACK = 5,
        _URC_HANDLER_FOUND = 6,
        _URC_INSTALL_CONTEXT = 7,
        _URC_CONTINUE_UNWIND = 8
    } _Unwind_Reason_Code;
    
    struct _Unwind_Context;
    struct _Unwind_Exception;

    typedef int _Unwind_Action;
    typedef void (*_Unwind_Exception_Cleanup_Fn)(_Unwind_Reason_Code, struct _Unwind_Exception *);
    typedef _Unwind_Reason_Code (*_Unwind_Stop_Fn)(int, _Unwind_Action, ulong, struct _Unwind_Exception *, struct _Unwind_Context *, void *);
    
    struct _Unwind_Exception {
        ulong exceptionClass;
        _Unwind_Exception_Cleanup_Fn exceptionCleanup;
        ulong private1;
        ulong private2;
    } __attribute__((__aligned__));
    
}

#endif