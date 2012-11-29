#include "Runtime.h"

extern "C" void __cxa_pure_virtual() { }

extern "C" {
    int __cxa_atexit(void (*f)(void *), void *p, void *d);
    void __cxa_finalize(void *d);
};

void *__dso_handle(0);
void *__stack_chk_guard(0);

namespace __cxxabiv1 {
    __extension__ typedef int __guard __attribute__((mode(__DI__)));

    extern "C" int __cxa_guard_acquire(__guard *Guard)              { return !(*(char *) (Guard)); }
    extern "C" void __cxa_guard_release(__guard *Guard)             { (*(char *) Guard) = 1; }
    extern "C" void __cxa_guard_abort(__guard *)                    { }
}

struct object {
    void (*f)(void*);
    void *p;
    void *d;
} object[32] = {{0,0,0}};

unsigned int iObject = 0;

int __cxa_atexit(void (*f)(void *), void *p, void *d) {
    if (iObject >= 32) 
        return -1;
       
    object[iObject].f = f;
    object[iObject].p = p;
    object[iObject].d = d;
    ++iObject;
    return 0;
}

void __cxa_finalize(void *d) {
    d=d;
    unsigned int i = iObject;
    
    for (;i>0;--i) {
        --iObject;
        object[iObject].f(object[iObject].p);
    }
}

void construct() {
    extern void (*__CTOR_LIST__)();
    void (**constructor)() = &__CTOR_LIST__;
    int total = *(int *)constructor;
    constructor++;
    
    while (total) {
        (*constructor)();
        total--;
        constructor++;
    }
}

void destruct() {
    extern void (*__DTOR_LIST__)();
    void (**deconstructor)() = &__DTOR_LIST__;
    int total = *(int *)deconstructor;
    deconstructor++;

    while (total) {
        (*deconstructor)();
        total--;
        deconstructor++;
    }
}

extern "C" void __stack_chk_guard_setup() {
    unsigned char *Guard;
    Guard = (unsigned char *) &__stack_chk_guard;
    Guard[sizeof(__stack_chk_guard) - 1] = 255;
    Guard[sizeof(__stack_chk_guard) - 2] = '\n';
    Guard[0] = 0;
}

extern "C" void __attribute__((noreturn)) __stack_chk_fail() {
    for(;;);
}

