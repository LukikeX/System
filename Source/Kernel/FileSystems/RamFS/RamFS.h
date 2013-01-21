#ifndef RAMFS_H
#define RAMFS_H

#include <VFS/FileSystem.proto.h>
#include <VFS/DirectoryNode.h>

class RamFS : public FileSystem {
private:
    virtual ~RamFS();
    RamFS(const RamFS& other);
    RamFS();
    bool unmount() { return true; }
    
    uint maxSize, usedSize;
    
public:
    static RamFS* mount(uint size, DirectoryNode* mountPoint);
    
    bool setUid(FSNode*, uint) { return true; }
    bool setGid(FSNode*, uint) { return true; }
    bool setName(FSNode*, String) { return true; }
    bool setPermissions(FSNode*, uint) { return true; }
    bool setParent(FSNode*, FSNode* parent);
    
    uint read(FileNode* file, ulong pos, ulong length, char* data);
    bool write(FileNode* file, ulong pos, ulong length, char* data);
    bool truncate(FileNode* file);
    
    bool loadContents(DirectoryNode*) { return true; }
    FileNode* createFile(DirectoryNode* parent, String name);
    DirectoryNode* createDirectory(DirectoryNode* parent, String name);
    bool remove(DirectoryNode*, FSNode* node);
    Partition* getPart() { return 0; }
};

#endif