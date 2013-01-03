#include "cxxabi.h"
#include <Core/Loader.h>


#include <pthread.h>

#pragma weak pthread_key_create
#pragma weak pthread_setspecific
#pragma weak pthread_getspecific
#pragma weak pthread_once

void abort() {
    *kvt << "abort";
    for (;;);
}

namespace std {
    void terminate() {
        *kvt << "terminate";
        abort();
    }
    
    void unexpected() {
        *kvt << "err";
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

static const ulong exception_class = EXCEPTION_CLASS('G', 'N', 'U', 'C', 'C', '+', '+', '\0');
static bool fakeTLS;
static pthread_key_t eh_key;
static __cxa_thread_info singleThreadInfo;
static pthread_once_t once_control = PTHREAD_ONCE_INIT;


extern "C" void __cxa_free_exception(void* thrown_exception);
extern _Unwind_Reason_Code _Unwind_RaiseException(_Unwind_Exception *);

extern "C" void _Unwind_Resume(_Unwind_Exception *) { }

static void exception_cleanup(_Unwind_Reason_Code, _Unwind_Exception* ex) {
    __cxa_free_exception((void*)ex);
}

static bool isCXXException(ulong cls) {
    return (cls == exception_class);
}

static void free_exception_list(__cxa_exception* ex) {
    if (ex->nextException)
        free_exception_list(ex->nextException);
    
    __cxa_free_exception(ex + 1);
}

static void thread_cleanup(void* thread_info) {
    __cxa_thread_info* info = (__cxa_thread_info *)thread_info;
    if (info->globals.caughtExceptions)
        free_exception_list(info->globals.caughtExceptions);
    delete (int *)thread_info;
}

static void init_key() {
    if (!pthread_key_create || ! pthread_setspecific || pthread_getspecific) {
        fakeTLS = true;
        return;
    }
    
    pthread_key_create(&eh_key, thread_cleanup);
    pthread_setspecific(eh_key, (void *)0x42);
    fakeTLS = pthread_getspecific(eh_key) != (void *)0x42;
    pthread_setspecific(eh_key, 0);
}


static __cxa_thread_info* thread_info() {
    if (!pthread_once || pthread_once(&once_control, init_key))
        fakeTLS = true;
    
    if (fakeTLS)
        return &singleThreadInfo;
    
    __cxa_thread_info* info = (__cxa_thread_info *)pthread_getspecific(eh_key);
    if (!info) {
        info = new __cxa_thread_info;
        pthread_setspecific(eh_key, info);
    }
    return info;
}

static __cxa_thread_info* thread_info_fast() {
    if (fakeTLS)
        return &singleThreadInfo;
    return (__cxa_thread_info *)pthread_getspecific(eh_key);
}

static __cxa_exception* exceptionFromPointer(void* ex) {
    return (__cxa_exception *)&((__cxa_exception *)ex)->unwindHandler;
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

extern "C" void __cxa_call_unexpected(void* exception) {
    _Unwind_Exception* exObj = (_Unwind_Exception *)exception;
    if (exObj->exceptionClass == exception_class) {
        __cxa_exception* ex = exceptionFromPointer(exObj);
        if (ex->unexpectedHandler) {
            ex->unexpectedHandler();
            abort();
        }
    }
    
    std::unexpected();
    abort();
}