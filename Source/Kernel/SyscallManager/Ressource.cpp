#include "Ressource.h"
#include "Res.h"

Ressource::Ressource(uint type, callT* callTable) : lock(MUTEX_FALSE) {
    id = Res::registerRes(this);
    this->type = type;
    callTables = 0;
    
    addCallTable(callTable);
}

Ressource::~Ressource() {
    Res::unregisterRes(id);
    delete callTables;
}

void Ressource::addCallTable(callT* callTable) {
    if (callTable)
        callTables = callTables->cons(callTable);
}

ulong Ressource::doCall(uint id, ulong a, ulong b, ulong c, ulong d, ulong e) {
    for (List<callT *>* iter = callTables; iter; iter = iter->next()) {
        callT* ct = iter->v();
        
        ulong i = 0;
        while (ct[i].id) { 
            callT& ce = ct[i];
            if (ce.id == id) {
                if (ce.params == 0) return (this->*(ce.c0))();
                if (ce.params == 1) return (this->*(ce.c1))(a);
                if (ce.params == 2) return (this->*(ce.c2))(a, b);
                if (ce.params == 3) return (this->*(ce.c3))(a, b, c);
                if (ce.params == 4) return (this->*(ce.c4))(a, b, c, d);
                if (ce.params == 5) return (this->*(ce.c5))(a, b, c, d, e);
            }
            i++;
        }
    }
    return (ulong)-1;
}

ulong Ressource::call(uint id, ulong a, ulong b, ulong c, ulong d, ulong e) {
    if (!accessible())
        return (ulong)-1;
    
    if (!id)
        return type;
    
    lock.waitLock();
    ulong r = doCall(id, a, b, c, d, e);
    lock.unlock();
    return r;
}