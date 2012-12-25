#include "VirtualTerminal.proto.h"
#include "TaskManager/Task.h"

VirtualTerminal::callT VirtualTerminal::callTable[] = {
    CALL1(VTIF_WRITE,       &VirtualTerminal::writeSC),
    CALL1(VTIF_PUT,         &VirtualTerminal::putSC),
    CALL1(VTIF_READLINE,    &VirtualTerminal::readLineSC),
    CALL1(VTIF_GETKEYPRESS, &VirtualTerminal::getKeyPressSC),
    CALL0(VTIF_ISBOXED,     &VirtualTerminal::isBoxedSC),
    CALL0(VTIF_GETHEIGHT,   &VirtualTerminal::getHeightSC),
    CALL0(VTIF_GETWIDTH,    &VirtualTerminal::getWidthSC),
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

ulong VirtualTerminal::putSC(ulong code) {
    put(WChar(code));
    return 0;
}

ulong VirtualTerminal::readLineSC(ulong show) {
    return readLine(show).serialize();
}

ulong VirtualTerminal::getKeyPressSC(ulong flags) {
    keyStatus ks = getkeyPress(flags & 1, flags & 2);
    void* ptr = Memory::mkXchgSpace(sizeof(keyStatus));
    Memory::copy((const uchar *)&ks, (uchar *)ptr, sizeof(keyStatus));
    return (ulong)ptr;
}

ulong VirtualTerminal::isBoxedSC() {
    return isBoxed();
}

ulong VirtualTerminal::getHeightSC() {
    return height();
}

ulong VirtualTerminal::getWidthSC() {
    return width();
}