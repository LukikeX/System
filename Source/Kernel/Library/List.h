#ifndef LIST_H
#define LIST_H

#include <Core/typedef.h>

template<typename T>
class List {
protected:
    T value;
    List<T>* m_next;
    
public:
    List(const T& value, List<T>* next = 0): value(value), m_next(next) { }
    ~List() {
        if (m_next)
            delete m_next;
    }
    
    T& v() { return value; }
    T& operator *() { return value; }
    
    List<T>* cons(const T& value) {
        return new List<T>(value, this);
    }
    
    List<T>* addAtEnd(const T& value) {
        if (!this)
            return new List<T>(value);
        else {
            if (!m_next) {
                m_next = new List<T>(value);
                return m_next;
            } else
                return m_next->addAtEnd(value);
        }
    }
    
    List<T>* next() {
        return m_next;
    }
    
    List<T>* last() {
        if (!m_next)
            return this;
        return m_next->last();
    }
    
    List<T>* deleteThis() {
        List<T>* ret = m_next;
        delete this;
        return ret;
    }
    
    void deleteNext() {
        if (!m_next)
            return;
        
        List<T>* tmp = m_next;
        m_next = m_next->m_next;
        delete tmp;
    }
    
    List<T>* removeOnce(const T& value) {
        if (!this)
            return 0;
        
        if (value == this->value)
            deleteThis();
        
        for (List<T> *iter = this; iter->next(); iter = iter->next()) {
            if (iter->next()->value == value) {
                iter->deleteNext();
                break;
            }
        }
        return this;
    }
    
    bool isEnd() {
        return next == 0;
    }
    
    uint size() {
        if (!m_next) { 
            return 1;
        }
        return m_next->size() + 1;
    }
};

#endif