#ifndef MUTEX_H
#define MUTEX_H

#include "typedef.h"

#define MUTEX_FALSE 0
#define MUTEX_TRUE  1

class Mutex {
private:
    ulong m_locked;
    ulong atomicExchange(ulong* ptr, ulong newVal);
    
public:
    Mutex(ulong locked = MUTEX_FALSE) { m_locked = locked; }
    bool lock();
    void unlock() { m_locked = MUTEX_FALSE; }
    bool locked() { return m_locked == MUTEX_TRUE; }
    void waitLock();
};

#endif