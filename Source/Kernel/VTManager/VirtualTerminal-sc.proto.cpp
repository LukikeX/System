#include "VirtualTerminal.proto.h"
#include "TaskManager/Task.h"

VirtualTerminal::callT VirtualTerminal::callTable[] = {
    CALL1(VTIF_WRITE, &VirtualTerminal::writeSC),
    CALL0(0, 0)
};

ulong VirtualTerminal::scall(uint wat, ulong, ulong, ulong, ulong) {
    if (wat == VTIF_SGETPRINVT)
        return Task::currentProcess()->getInVT()->resId();
    else if (wat == VTIF_SGETPROUTVT)
        return Task::currentProcess()->getOutVT()->resId();
    
    return (ulong)-1;
}

ulong VirtualTerminal::writeSC(ulong wat) {
    if (wat) {
        String *s = (String *)wat;
        write(*s);
    }
    return 0;
}
