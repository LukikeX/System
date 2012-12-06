#include "Mutex.h"
#include <TaskManager/Task.h>

ulong Mutex::atomicExchange(ulong* ptr, ulong newVal) {
    ulong ret;
    asm volatile ("xchg (%%rcx), %%rax" : "=a"(ret) : "c"(ptr), "a"(newVal));
    return ret;
}

bool Mutex::lock() {
    if (atomicExchange(&m_locked, MUTEX_TRUE) == MUTEX_TRUE)
        return false;
    return true;
}

void Mutex::waitLock() {
    while (atomicExchange(&m_locked, MUTEX_TRUE) == MUTEX_TRUE) {
   //     if (Task::currentThread())
    //        Task::currentThread()->sleep(10);
     //   else
            return;
    }
}