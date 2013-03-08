#include "DirectoryNode.h"

DirectoryNode::callT DirectoryNode::callTable[] = {
    CALL1(FNIF_GETIDXCHILD, &DirectoryNode::getIdxChildSC),
    CALL1(FNIF_GETNAME, &DirectoryNode::getNameChildSC),
    CALL0(0, 0)
};

ulong DirectoryNode::getIdxChildSC(ulong idx) {
    if (!runnable())
        return (ulong)-1;
    
    FSNode* n = getChild(idx);
    if (n)
        return n->resId();
    return (ulong)-1;
}

ulong DirectoryNode::getNameChildSC(ulong name) {
    if (!runnable())
        return (ulong)-1;
    
    String* s = (String *)name;
    FSNode* n = getChild(*s);
    if (n)
        return n->resId();
    return (ulong)-1;
}

DirectoryNode::~DirectoryNode() {
    if (contentLoaded) {
        for (uint i = 0; i < children.size(); i++)
            delete children[i];
    }
    
    if (name == "/" && parent)
        ((DirectoryNode *)parent)->unmount();
}

String DirectoryNode::getName() const {
    if (name == "/" && parent)
        return parent->getName();
    return name;
}

ulong DirectoryNode::getLength() {
    if (mounts)
        return mounts->getLength();
    
    if (!contentLoaded && !loadContent())
        return false;
    return length;
}

FSNode* DirectoryNode::getParent() const {
    if (name == "/" && parent)
        return parent->getParent();
    return parent;
}

bool DirectoryNode::removable() {
    if (!contentLoaded && !loadContent())
        return false;
    return children.empty() && !mounts;
}

bool DirectoryNode::unmountable() {
    if (!contentLoaded)
        return true;
    
    if (mounts)
        return false;
    
    for (uint i = 0; i < children.size(); i++) {
        if (children[i]->type() == FS_MOUNTPOINT) {
            if (!((DirectoryNode *)children[i])->unmountable())
                return false;
            else if (!children[i]->removable())
                return false;
        }
    }
    return true;
}

bool DirectoryNode::mountpointable() {
    if (!contentLoaded && !loadContent())
        return false;
    
    return children.empty();
}

bool DirectoryNode::loadContent() {
    if (mounts)
        return mounts->loadContent();
    
    if (contentLoaded)
        return true;
    
    bool v = fs->loadContents(this);
    if (!v)
        return false;
    
    length = children.size();
    contentLoaded = true;
    return v;
}

FSNode* DirectoryNode::getChild(uint idx) {
    if (mounts)
        return mounts->getChild(idx);
    
    if (!contentLoaded && !loadContent())
        return 0;
    
    if (idx >= children.size())
        return 0;
    
    return children[idx];
}

FSNode* DirectoryNode::getChild(const String& name) {
    if (mounts)
        return mounts->getChild(name);
    
    if (!contentLoaded && !loadContent())
        return 0;
    
    for (uint i = 0; i < children.size(); i++) {
        if (name == children[i]->getName())
            return children[i];
    }
    return 0;
}

Vector<FSNode *>& DirectoryNode::getChildren() {
    if (mounts)
        return mounts->getChildren();
    
    if (!contentLoaded)
        loadContent();
    
    return children;
}

DirectoryNode* DirectoryNode::createDirectory(const String& name) {
    if (mounts)
        return mounts->createDirectory(name);
    
    DirectoryNode* ret = fs->createDirectory(this, name);
    length = children.size();
    return ret;
}

bool DirectoryNode::remove(FSNode* child) {
    if (mounts)
        return mounts->remove(child);
    
    if (!contentLoaded && !loadContent())
        return false;
    
    uint idx = (uint)-1;
    for (uint i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            idx = i;
            break;
        }
    }
    
    if (idx == (uint)-1)
        return false;
    
    if (!children[idx]->removable())
        return false;
    
    if (fs != children[idx]->getFS())
        return false;
    
    if (!fs->remove(this, child))
        return false;
    
    children[idx] = children.back();
    children.pop();
    length--;
    
    delete child;
    return true;
}

FileNode* DirectoryNode::createFile(const String name) {
    if (mounts)
        return mounts->createFile(name);
    
    FileNode* ret = fs->createFile(this, name);
    length = children.size();
    return ret;
}