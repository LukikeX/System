#include "File.h"
#include "SyscallManager/Res.h"
#include "VTManager/VirtualTerminal.proto.h"

File::callT File::callTable[] = {
    CALL0(FLIF_CLOSE,    &File::closeSC),
    CALL0(FLIF_VALID,    &File::validSC),
    CALL2(FLIF_READ,     &File::readSC),
    CALL2(FLIF_WRITE,    &File::writeSC),
    CALL2(FLIF_SEEK,     &File::seekSC),
    CALL0(FLIF_POSITION, &File::positionSC),
    CALL0(FLIF_LENGTH,   &File::lengthSC),
    CALL0(FLIF_EOF,      &File::eofSC),
    CALL0(FLIF_RESET,    &File::resetSC),
    CALL0(0, 0)
};

ulong File::scall(uint wat, ulong a, ulong b, ulong c, ulong) {
    if (wat == FLIF_SOPEN) {
        String* name = (String *)a;
        FSNode* start = Res::get<FSNode>(c, FNIF_OBJTYPE);
        File* f = new File();
        if (!f->open(*name, b, start, true))
            delete f;
        else
            return f->resId();
    }
    return (ulong)-1;
}

ulong File::closeSC() {
    if (!isValid())
        return 1;
    close();
    return 0;
}

ulong File::validSC() {
    return isValid() ? 1 : 0;
}

ulong File::readSC(ulong length, ulong ptr) {
    if (!file->readable())
        return 0;
    return read(length, (char *)ptr);
}

ulong File::writeSC(ulong length, ulong ptr) {
    if (!file->writable())
        return 0;
    return write(length, (char *)ptr) ? 1 : 0;
}

ulong File::seekSC(ulong count, ulong mode) {
    return (seek(count, mode) ? 1 : 0);
}

ulong File::positionSC() {
    ulong* w = (ulong *)Memory::mkXchgSpace(sizeof(ulong));
    *w = position;
    return (ulong)w;
}

ulong File::lengthSC() {
    ulong* w = (ulong *)Memory::mkXchgSpace(sizeof(ulong));
    *w = getLength();
    return (ulong)w;
}

ulong File::eofSC() {
    return eof() ? 1 : 0;
}

ulong File::resetSC() {
    reset();
    return 0;
}

bool File::accessible() {
    //kontrola  Usr::uid() == m_process->getUid()) return true;
    return true;
}