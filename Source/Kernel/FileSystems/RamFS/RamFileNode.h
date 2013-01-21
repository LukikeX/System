#ifndef RAMFILENODE_H
#define RAMFILENODE_H

#include <VFS/FileNode.proto.h>

class RamFileNode : public FileNode {
    friend class RamFS;
private:
    char* data;
    
    RamFileNode(String name, FileSystem* fs, FSNode *parent, uint perm = 0777,
            uint uid = 0, uint gid = 0): FileNode(name, fs, parent, perm, uid, gid), data(0) { }
    ~RamFileNode() { if (data) delete[] data; }
    
    void setLength(uint len) { length = len; }
};

#endif