#ifndef FSNODE_PROTO_H
#define	FSNODE_PROTO_H

#include "FileSystem.proto.h"
#include <SyscallManager/Ressource.h>
#include <UserManager/User.h>

class FSNode : public Ressource {
protected:
    String name;
    FileSystem* fs;
    FSNode* parent;
    
    ulong length;
    uint permissions, uid, gid;
    
public:
    enum {
        FS_FILE        = 1,
        FS_DIRECTORY   = 2,
        FS_CHARDEVICE  = 4,
        FS_BLOCKDEVICE = 8,
        FS_PIPE        = 16,
        FS_SYMLINK     = 32,
        FS_APPLICATION = 64,
        FS_MOUNTPOINT  = 128
    };
    
    FSNode(String name, FileSystem* fs, FSNode* parent, ulong length = 0, 
           uint perms = 0777, uint uid = 0, uint gid = 0) : 
        Ressource(FNIF_OBJTYPE, callTable), name(name), fs(fs), parent(parent), 
        length(length), permissions(perms), uid(uid), gid(gid) { }
    virtual ~FSNode() { }
    
    virtual uchar type() const = 0;
    virtual bool removable() = 0;
    virtual bool used() const { return false; }
    
    virtual String getName() const { return name; }
    virtual ulong getLength() const { return length; }
    uint getPermissions() const { return permissions; }
    uint getUid() const { return uid; }
    uint getgid() const { return gid; }
    FileSystem* getFS() const { return fs; }
    virtual FSNode* getParent() const { return parent; }
    
    bool readable(User* user = 0);
    bool writable(User* user = 0);
    bool runnable(User* user = 0);
    
    bool setPermissions(uint perm) {
        bool v = fs->setPermissions(this, perm);
        if (v)
            this->name = name;
        return v;
    }
    
    bool setUid(uint uid) {
        bool v = fs->setUid(this, uid);
        if (v)
            this->uid = uid;
        return v;
    }
    
    bool setGid(uint gid) {
        bool v = fs->setGid(this, gid);
        if (v)
            this->gid = gid;
        return v;
    }
    
    bool setParent(FSNode* parent) {
        bool v = fs->setParent(this, parent);
        if (v)
            this->parent = parent;
        return v;
    }
    
    //Syscalls:
    static ulong scall(uint wat, ulong a, ulong b, ulong, ulong);
    
private:
    static callT callTable[];
    bool accessible();
    ulong getNameSC();
    ulong getLengthC();
    ulong typeSC();
    ulong getParentSC();
    ulong getPathSC();
    ulong setCwdSC();
    ulong removeSC();
};

#endif