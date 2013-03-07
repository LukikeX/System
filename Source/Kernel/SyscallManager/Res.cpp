#include "Res.h"
#include "MemoryManager/Memory.h"
#include "TaskManager/Process.h"
#include "TaskManager/Thread.h"
#include "Devices/ATA/ATAController.h"
#include "Core/System.h"
#include <VTManager/VirtualTerminal.proto.h>

Ressource** Res::ressources = 0;
uint Res::size = 0;

Res::staticCallT Res::staticCalls[] = {
    {VTIF_OBJTYPE, VirtualTerminal::scall},
    {PRIF_OBJTYPE, Process::scall},
    {THIF_OBJTYPE, Thread::scall},
    {FLIF_OBJTYPE, File::scall},
    {FNIF_OBJTYPE, FSNode::scall},
    {SYIF_OBJTYPE, System::scall},
    {0, 0}
};

void Res::unregisterRes(uint id) {
    ressources[id] = 0;
}

uint Res::registerRes(Ressource* r) {
    if (!ressources || !size) {
        ressources = (Ressource **)new ulong[32];
        size = 32;
        
        for (uint i = 0; i < 32; i++)
            ressources[i] = 0;
    }
    
    for (uint i = 0; i < size; i++) {
        if (!ressources[i]) {
            ressources[i] = r;
            return i;
        }
    }
    
    expand();
    return registerRes(r);
}

void Res::expand() {
    size += 32;
    Ressource** tmp = (Ressource **)new ulong[size];
    
    for (ulong i = 0; i < size - 32; i++) {
        if (i < size - 32)
            tmp[i] = ressources[i];
        else
            tmp[i] = 0;
    }
    
    delete[] ressources;
    ressources = tmp;
}

ulong Res::call(uint ressource, uint wat, ulong a, ulong b, ulong c, ulong d, ulong e) {
    if (ressource == 0xFFFFFFFE) {
        for (ulong i = 0; staticCalls[i].id != 0; i++) {
            if (staticCalls[i].id == a) 
                return staticCalls[i].call(wat, b, c, d, e);
        }
        return 0;
    } else {
        if (ressource > size || !ressources[ressource])
            return (ulong)-1;
        else
            return ressources[ressource]->call(wat, a, b, c, d, e);
    }
}