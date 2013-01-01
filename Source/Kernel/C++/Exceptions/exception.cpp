#include "cxxabi.h"
#include <Core/Loader.h>

extern "C" void __cxa_free_exception(void* thrown_exception);

void abort() {
    for (;;);
}

namespace std {
    void terminate() {
        abort();
    }
}

#define EXCEPTION_CLASS(a,b,c,d,e,f,g,h) \
	(((ulong)a << 56) +\
	 ((ulong)b << 48) +\
	 ((ulong)c << 40) +\
	 ((ulong)d << 32) +\
	 ((ulong)e << 24) +\
	 ((ulong)f << 16) +\
	 ((ulong)g << 8) +\
	 ((ulong)h))


using namespace __cxxabiv1;

extern "C" void _Unwind_Resume(_Unwind_Exception *) {

}
extern _Unwind_Reason_Code _Unwind_RaiseException(_Unwind_Exception*);

static const ulong exception_class = EXCEPTION_CLASS('G', 'N', 'U', 'C', 'C', '+', '+', '\0');

static void exception_cleanup(_Unwind_Reason_Code, _Unwind_Exception* ex) {
    __cxa_free_exception((void*)ex);
}

static bool isCXXException(ulong cls) {
    return (cls == exception_class);
}

static __cxa_thread_info* thread_info() {
    return (__cxa_thread_info *)0; //opravit...
}

static __cxa_thread_info* thread_info_fast() {
    return (__cxa_thread_info *)0; //opravit...
}

static __cxa_exception* exceptionFromPointer(void* ) {//ex
    return (__cxa_exception *)0; //opravit
}

static void releaseException(__cxa_exception* exception) {
    __cxa_free_exception(exception + 1);
}

extern "C" __cxa_eh_globals* __cxa_get_globals() {
    return &(thread_info()->globals);
}

extern "C" __cxa_eh_globals* __cxa_get_globals_fast() {
    return &(thread_info_fast()->globals);
}


static void throw_exception(__cxa_exception* ex) {
    __cxa_thread_info* info = thread_info();
    ex->unexpectedHandler = info->unexpectedHandler;
    if (!ex->unexpectedHandler)
        ex->unexpectedHandler = std::terminate;
    *kvt << "lol";
    for (;;);
    ex->terminateHandler = info->terminateHandler;
    if (!ex->terminateHandler)
        ex->terminateHandler = abort;
    
    info->globals.uncaughtExceptions++;
    //_Unwind_Reason_Code err = _Unwind_RaiseException(&ex->unwindHandler);
    //report_failure(err, ex);
}


extern "C" void* __cxa_allocate_exception(unsigned int size) {
    char* buffer = new char[size + sizeof(__cxa_exception)];
    return (void *)(buffer + sizeof(__cxa_exception));
}

extern "C" void __cxa_throw(void* thrown_exception, std::type_info* info, void(*dest)(void *)) {
    __cxa_exception* ex = ((__cxa_exception *)thrown_exception) - 1;
    
    ex->referenceCount = 1;
    ex->exceptionType = info;
    ex->exceptionDestructor = dest;
    ex->unwindHandler.exceptionClass = exception_class;
    ex->unwindHandler.exceptionCleanup = exception_cleanup;
    
    throw_exception(ex);
}

extern "C" void __cxa_free_exception(void* thrown_exception) {
    __cxa_exception* ex = ((__cxa_exception *)thrown_exception) - 1;
    
    if (ex->exceptionDestructor) {
        try {
            ex->exceptionDestructor(thrown_exception);
        } catch (...) {
            std::terminate();
        }
    }
    delete ex;
}

//==============================================================================
extern "C" void* __cxa_get_exception_ptr(void* exceptionObject) {
    return exceptionFromPointer(exceptionObject)->adjustedPtr;
}

extern "C" void* __cxa_begin_catch(void* e) throw() {
    __cxa_eh_globals* globals = __cxa_get_globals();
    globals->uncaughtExceptions--;
    _Unwind_Exception* exceptionObject = (_Unwind_Exception *)e;
    
    if (isCXXException(exceptionObject->exceptionClass)) {
        __cxa_exception* ex = exceptionFromPointer(exceptionObject);
        
        if (!ex->handlerCount) {
            ex->nextException = globals->caughtExceptions;
            globals->caughtExceptions = ex;
        }
        
        if (ex->handlerCount < 0)
            ex->handlerCount = -ex->handlerCount + 1;
        else
            ex->handlerCount++;
        return ex->adjustedPtr;
    }
    return ((char *)exceptionObject + sizeof(_Unwind_Exception));
}

extern "C" void __cxa_end_catch() {
    __cxa_eh_globals* globals = __cxa_get_globals_fast();
    __cxa_exception* ex = globals->caughtExceptions;
    
    bool deleteException = true;
    
    if (ex->handlerCount < 0) {
        ex->handlerCount++;
        deleteException = false;
    } else
        ex->handlerCount--;
    
    if (!ex->handlerCount) {
        globals->caughtExceptions = ex->nextException;
        if (deleteException)
            releaseException(ex);
    }
}