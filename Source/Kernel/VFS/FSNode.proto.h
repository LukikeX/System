#ifndef FSNODE_PROTO_H
#define	FSNODE_PROTO_H

#include "FileSystem.proto.h"

class FSNode {
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
        FS_CHARDEVICE  = 3,
        FS_BLOCKDEVICE = 4,
        FS_PIPE        = 5,
        FS_SYMLINK     = 6,
        FS_APPLICATION = 7,
        FS_MOUNTPOINT  = 8
    };
    
    FSNode(String name, FileSystem* fs, FSNode* parent, ulong length = 0, 
           uint perms = 0777, uint uid = 0, uint gid = 0) : name(name), fs(fs), 
        parent(parent), length(length), permissions(perms), uid(uid), gid(gid) { }
    virtual ~FSNode() { }
    
    virtual uchar type() const = 0;
    virtual bool removable() const = 0;
    virtual bool used() const { return false; }
    
    virtual String getName() const { return name; }
    virtual ulong getLength() const { return length; }
    uint getPermissions() const { return permissions; }
    uint getUid() const { return uid; }
    uint getgid() const { return gid; }
    FileSystem* getFS() const { return fs; }
    virtual FSNode* getParent() const { return parent; }
    
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
};

#endif