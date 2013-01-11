#ifndef RES_H
#define RES_H

#include "Ressource.h"

typedef ulong (*staticcall)(uint, ulong, ulong, ulong, ulong);

class Res {
public: //opravit
    static Ressource** ressources;
    static uint size;
    
    static void expand();
    
public:
    struct staticCallT {
        uint id;
        staticcall call;
    };
    
    static uint registerRes(Ressource* r);
    static void unregisterRes(uint id);
    static staticCallT staticCalls[];
    
    template <typename T>
    inline static T* get(ulong id, ulong type) {
        if (id > size || !ressources[id])
            return 0;
        
        if (ressources[id]->resType() != type)
            return 0;
        
        return (T *)ressources[id];
    }
    
    static ulong call(uint ressource, uint wat, ulong a, ulong b, ulong c, ulong d, ulong e);
};

#endif