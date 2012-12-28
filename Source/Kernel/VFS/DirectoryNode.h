#ifndef DIRECTORYNODE_H
#define	DIRECTORYNODE_H

#include "FSNode.proto.h"
#include <Library/Vector.h>

class DirectoryNode : public FSNode {
protected:
    Vector<FSNode *> children;
    bool contentLoaded;
    DirectoryNode* mounts;
    
public:
    DirectoryNode(String name, FileSystem* fs, FSNode* parent, uint perms = 0777,
                  uint uid = 0, uint gid = 0) : FSNode(name, fs, parent, 0, perms, uid, gid),
        children(), contentLoaded(false), mounts(0) { }
    virtual ~DirectoryNode();
    
    Vector<FSNode *>& getChildren();
    uchar type() const { return mounts ? FS_MOUNTPOINT : FS_DIRECTORY; }
    String getName() const;
    ulong getLength();
    FSNode* getParent() const;
    bool removable();
    bool unmountable();
    bool mountpointable();
    void mount(DirectoryNode* childRoot) { mounts = childRoot; }
    void unmount() { mounts = 0; }
    
    bool loadContent();
    FSNode* getChild(uint idx);
    FSNode* getChild(const String& name);
    FileNode* createFile(const String name);
    DirectoryNode* createDirectory(const String& name);
    bool remove(FSNode* child);
    
    Vector<FSNode *>& FSGetChildren() { return children; }
};

#endif