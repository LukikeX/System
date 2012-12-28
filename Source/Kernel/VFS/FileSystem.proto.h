#ifndef FILESYSYTEM_PH
#define FILESYSYTEM_PH

#include "Partition.h"

class FSNode;
class FileNode;
class DirectoryNode;

class FileSystem {
    //friend class VFS;
private:
    String identifier;
    
protected:
    virtual ~FileSystem();
    bool isWritable;
    DirectoryNode* rootNode;
    virtual bool unmount() = 0;
    
public:
    bool isWritable() { return isWritable; }
    DirectoryNode* getRootNode() { return rootNode; }
    
    virtual bool setName(FSNode* node, String name) = 0;
    virtual bool setPermissions(FSNode* node, uint perms) = 0;
    virtual bool setUid(FSNode* node, uint uid) = 0;
    virtual bool setGid(FSNode* node, uint gid) = 0;
    virtual bool setParent(FSNode* node, FSNode* parent) = 0;
    
    virtual uint read(FileNode* file, ulong start, ulong length, char* data) = 0;
    virtual bool write(FileNode* file, ulong start, ulong length, char* data) = 0;
    virtual bool truncate(FileNode* file) = 0;
    
    virtual bool loadContents(DirectoryNode* dir) = 0;
    virtual FileNode* createFile(DirectoryNode* parent, String name) = 0;
    virtual DirectoryNode* createDirectory(DirectoryNode* parent, String name) = 0;
    virtual bool remove(DirectoryNode* parent, FSNode* node) = 0;
    
    virtual Partititon* getPart() = 0;
    void setIdentifier(String idn) { identifier = idn; }
    String getIdentifier() const { return identifier; }
};

#endif