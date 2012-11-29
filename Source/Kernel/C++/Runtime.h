#ifndef RUNTIME_H
#define RUNTIME_H

extern "C" {
    int __cxa_atexit(void (*f)(void *), void *p, void *d);
    void __cxa_finalize(void *d);
};

void construct();
void destruct();

#endif