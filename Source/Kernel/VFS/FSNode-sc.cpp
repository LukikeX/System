#include "FSNode.proto.h"
#include "VFS.h"
#include <TaskManager/Task.h>
#include <SyscallManager/Res.h>
#include <UserManager/UserManager.h>

FSNode::callT FSNode::callTable[] = {
    CALL0(FNIF_GETNAME,   &FSNode::getNameSC),
    CALL0(FNIF_TYPE,      &FSNode::typeSC),
    CALL0(FNIF_GETPARENT, &FSNode::getParentSC),
    CALL0(FNIF_GETLENGTH, &FSNode::getLengthC),
    CALL0(FNIF_GETUID,    &FSNode::getUid),
    CALL0(FNIF_GETGID,    &FSNode::getgid),
    CALL0(FNIF_GETPERM,   &FSNode::getPermissions),
    CALL0(FNIF_GETPATH,   &FSNode::getPathSC),
    CALL0(FNIF_SETCWD,    &FSNode::setCwdSC),
    CALL0(FNIF_REMOVE,    &FSNode::removeSC),
    CALL0(0, 0)
};

ulong FSNode::scall(uint wat, ulong a, ulong b, ulong, ulong) {
    String* path = (String *)a;
    
    switch (wat) {
        case FNIF_SGETRFN:
            return VFS::getRootNode()->resId();
        case FNIF_SGETCWD:
            return Task::currentProcess()->getCwd()->resId();
        case FNIF_SFIND:        
            FSNode* nd;
            if (!b)
                nd = VFS::find(*path);
            else
                nd = VFS::find(*path, Res::get<DirectoryNode>(b, FNIF_OBJTYPE));
            if (nd)
                return nd->resId();
            break;
        case FNIF_SMKDIR:
            FSNode* n;
            if (!b)
                n = VFS::createDirectory(*path, 0, true);
            else
                n = VFS::createDirectory(*path, Res::get<DirectoryNode>(b, FNIF_OBJTYPE), true);
            
            if (n)
                return n->resId();
            break;
    }
    return (ulong)-1;
}

ulong FSNode::getNameSC() {
    return getName().serialize();
}

ulong FSNode::typeSC() {
    return type();
}

ulong FSNode::getLengthC() {
    ulong* a = (ulong *)Memory::mkXchgSpace(sizeof(ulong));
    *a = getLength();
    return (ulong)a;
}

ulong FSNode::getPathSC() {
    return VFS::path(this).serialize();
}

ulong FSNode::setCwdSC() {
    if (type() == FS_DIRECTORY)
        Task::currentProcess()->setCwd((DirectoryNode *)this);
    
    return 0;
}

ulong FSNode::removeSC() {
    if (!writable())
        return 0;
    return (VFS::remove(this) ? 1 : 0);
}

ulong FSNode::getParentSC() {
    if (parent)
        return parent->resId();
    return (ulong)-1;
}

bool FSNode::accessible() {
    return readable();
}

bool FSNode::readable(User* user) {
    if (ISROOT)
        return true;
    
    if (!user)
        user = UserManager::user();
    
    if (user->getUid() == uid)
        return ((permissions >> 6) & 4) != 0;
    if(user->isInGroup(gid))
        return ((permissions >> 3) & 4) != 0;
    return (permissions & 4) != 0;
}

bool FSNode::writable(User* user) {
    if (ISROOT)
        return true;
    
    if (!user)
        user = UserManager::user();
    
    if (user->getUid() == uid)
        return ((permissions >> 6) & 2) != 0;
    if(user->isInGroup(gid))
        return ((permissions >> 3) & 2) != 0;
    return (permissions & 2) != 0;
}

bool FSNode::runnable(User* user) {
    if (ISROOT)
        return true;
    
    if (!user)
        user = UserManager::user();
    
    if (user->getUid() == uid)
        return ((permissions >> 6) & 1) != 0;
    if(user->isInGroup(gid))
        return ((permissions >> 3) & 1) != 0;
    return (permissions & 1) != 0;
}