#include "FSNode.proto.h"
#include "VFS.h"
#include "TaskManager/Task.h"
#include "SyscallManager/Res.h"

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
    switch (wat) {
        case FNIF_SGETRFN:
            return VFS::getRootNode()->resId();
        //case FNIF_SGETCWD:
            //return Task::currentProcess()->getc
        case FNIF_SFIND:
            String* path = (String *)a;
            FSNode* n;
            if (!b)
                n = VFS::find(*path);
            else
                n = VFS::find(*path, Res::get<DirectoryNode>(b, FNIF_OBJTYPE));
            if (n)
                return n->resId();
            break;
        case FNIF_SMKDIR:
            String* path = (String *)a;
            FSNode* n;
            if (!b) {
                //n = VFS::createDirectory(
            }
    }
}