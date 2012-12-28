#include "VFS.h"
#include "DirectoryNode.h"

FileSystem::~FileSystem() {
    delete rootNode;
}

Vector<FileSystem *> VFS::fileSystems;
DirectoryNode* VFS::rootNode = 0;
VFS::localFST VFS::localFS[] = {
    {0, 0}
};

void VFS::registerFileSystem(FileSystem* fs) {
    unregisterFileSystem(fs);
    fileSystems.push(fs);
    
    if (!rootNode)
        rootNode = fs->getRootNode();
}

void VFS::unregisterFileSystem(FileSystem* fs) {
    for (uint i = 0; i < fileSystems.size(); i++) {
        if (fileSystems[i] == fs) {
            fileSystems[i] == fileSystems.back();
            fileSystems.pop();
            break;
        }
    }
}

bool VFS::unmount(FileSystem* fs) {
    if (!fs->getRootNode()->unmountable())
        return false;
    
    if (fs->getRootNode() == rootNode)
        return false;
    
    if (!fs->unmount())
        return false;
    
    delete fs;
    return true;
}

FSNode* VFS::find(const String& path, FSNode* start) {
    if (!start)
        start = rootNode;
    
    Vector<String> p = path.split('/');
    
    FSNode* node = start;
    if (p[0].empty())
        node = rootNode;
    
    for (uint i = 0; i < p.size(); i++) {
        if (p[i] == "..")
            node = node->getParent();
        else if (!p[i].empty() && p[i] != ".") {
            if (node->type() & (FSNode::FS_DIRECTORY | FSNode::FS_MOUNTPOINT))
                node = ((DirectoryNode *)node)->getChild(p[i]);
            else
                return 0;
        }
    }
    return node;
}

FSNode* VFS::createFile(const String& path, FSNode* start) {
    if (!start)
        start = rootNode;
    
    if (find(path, start))
        return 0;
    
    Vector<String> p = path.split('/');
    String name = p.back();
    p.pop();
    
    String s = ".";
    for (uint i = 0; i < p.size(); i++)
        s += String("/") + p[i];
    
    FSNode* node = find(s, start);
    if (!node)
        return 0;
    
    if (node->type() & (FSNode::FS_DIRECTORY | FSNode::FS_MOUNTPOINT))
        return (FSNode* )((DirectoryNode *)node)->createFile(name);
    return 0;
}

FSNode* VFS::createDirectory(const String& path, FSNode* start) {
    if (!start)
        start = rootNode;
    
    if (find(path, start))
        return 0;
    
    Vector<String> p = path.split('/');
    String name = p.back();
    p.pop();
    
    String s = ".";
    for (uint i = 0; i < p.size(); i++)
        s += String("/") + p[i];
    
    FSNode* node = find(s, start);
    if (!node)
        return 0;
    
    if (node->type() & (FSNode::FS_DIRECTORY | FSNode::FS_MOUNTPOINT))
        return ((DirectoryNode *)node)->createDirectory(name);
    return 0;
}

bool VFS::remove(FSNode* node) {
    FSNode* parent = node->getParent();
    
    if (!parent)
        return false;
    
    if (parent->type() & (FSNode::FS_DIRECTORY | FSNode::FS_MOUNTPOINT))
        return ((DirectoryNode *)parent)->remove(node);
    else
        return false;
}

bool VFS::remove(const String& path, FSNode* start) {
    FSNode* node = find(path, start);
    if (!node)
        return false;
    
    return remove(node);
}

String VFS::path(FSNode* node) {
    String path = "/";
    
    while (node) {
        String r = "/";
        r += node->getName();
        if (r != "//") {
            r += path;
            path = r;
        }
        node = node->getParent();
    }
    return path;
}

