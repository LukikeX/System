#ifndef FILENODE_H
#define	FILENODE_H

#include "FSNode.proto.h"

class FileNode : public FSNode {
    friend class File;
protected:
    uint writers, readers;
    
public:
    FileNode(String name, FileSystem* fs, FSNode* parent, uint length = 0, 
             uint perms = 0777, uint uid = 0, uint gid = 0) : 
        FSNode(name, fs, parent, length, perms, uid, gid), writers(0), readers(0) { }
    virtual ~FileNode() { }
        
    uchar type() const { return FS_FILE; }
    bool removable() const { return true; }
    bool used() const { return readers || writers; }
    
    uint read(ulong start, ulong length, char* data) {
        return fs->read(this, start, length, data);
    }
    
    bool write(ulong start, ulong length, char* data) {
        return fs->write(this, start, length, data);
    }
    
    bool truncate() {
        return fs->truncate(this);
    }
};

#endif