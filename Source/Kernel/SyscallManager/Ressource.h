#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <Core/typedef.h>
#include <Core/Mutex.h>
#include <Library/List.h>

class Ressource;

typedef ulong (Ressource::*call0)();
typedef ulong (Ressource::*call1)(ulong);
typedef ulong (Ressource::*call2)(ulong, ulong);
typedef ulong (Ressource::*call3)(ulong, ulong, ulong);
typedef ulong (Ressource::*call4)(ulong, ulong, ulong, ulong);
typedef ulong (Ressource::*call5)(ulong, ulong, ulong, ulong, ulong);

#define CALL0(id, ptr) {0, id, {c0: (call0)ptr}}
#define CALL1(id, ptr) {1, id, {c1: (call1)ptr}}
#define CALL2(id, ptr) {2, id, {c2: (call2)ptr}}
#define CALL3(id, ptr) {3, id, {c3: (call3)ptr}}
#define CALL4(id, ptr) {4, id, {c4: (call4)ptr}}
#define CALL5(id, ptr) {5, id, {c5: (call5)ptr}}


class Ressource {
public:
    struct callT {
        uchar params;
        uint id;
        union {
            call0 c0;
            call1 c1;
            call2 c2;
            call3 c3;
            call4 c4;
            call5 c5;
        };
    };
    
    ulong call(uint id, ulong a, ulong b, ulong c, ulong d, ulong e);
    inline uint resType() { return type; }
    inline uint resId() { return id; }
    
private:
    Ressource(const Ressource&);
    Ressource& operator=(const Ressource&);
    
    Mutex lock;
    
    ulong id;
    ulong type;
    List<callT *>* callTables;
    
    ulong doCall(uint id, ulong a, ulong b, ulong c, ulong d, ulong e);
    
protected:
    Ressource(uint type, callT* callTable = 0);
    virtual ~Ressource();
    
    virtual bool accessible() = 0;
    void addCallTable(callT* callTable);
};

#endif