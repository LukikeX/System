#include "RamFS.h"
#include "VFS/FileNode.proto.h"
#include "RamFileNode.h"
#include "VFS/File.h"
#include <VFS/VFS.h>

RamFS::RamFS() { }
RamFS::~RamFS() { }

RamFS* RamFS::mount(uint size, DirectoryNode* mountPoint) {
    if (mountPoint && !mountPoint->mountpointable())
        return 0;
    
    RamFS* rfs = new RamFS();
    rfs->maxSize = size;
    rfs->usedSize = 0;
    rfs->m_isWritable = true;
    rfs->rootNode = new DirectoryNode("/", rfs, mountPoint);
    
    if (mountPoint)
        mountPoint->mount(rfs->rootNode);
    
    VFS::registerFileSystem(rfs);
    return rfs;
}

bool RamFS::setParent(FSNode*, FSNode* parent) {
    if (parent->getFS() == this)
        return true;
    return false;
}

uint RamFS::read(FileNode* file, ulong pos, ulong length, char* data) {
    if (file->type() != FSNode::FS_FILE)
        return 0;
    
    RamFileNode* node = (RamFileNode *)file;
    if (file->getLength() <= pos)
        return 0;
    
    uint len = file->getLength() - pos;
    if (len > length)
        len = length;
    
    Memory::copy(node->data, data, length, pos);
    return len;
}

bool RamFS::write(FileNode* file, ulong pos, ulong length, char* data) {
    if (!isWritable())
        return false;
    
    if (file->type() != FSNode::FS_FILE)
        return false;
    
    RamFileNode* node = (RamFileNode *)file;
    uint end = pos + length;
    if (end > node->getLength()) {
        if (usedSize - node->getLength() + end > maxSize)
            return false;
        
        usedSize -= node->getLength();
        usedSize += end;
        
        char* data = new char[end];
        if (!data)
            return false;
        
        if (node->data) {
            Memory::copy(node->data, data, node->getLength());
            delete[] node->data;
        }
        node->data = data;
        node->setLength(end);
    }
    
    Memory::copy(data, node->data + pos, length);
    return true;
}

bool RamFS::truncate(FileNode* file) {
    if (!isWritable())
        return false;
    
    if (file->type() != FSNode::FS_FILE)
        return false;
    
    RamFileNode* node = (RamFileNode *)file;
    delete[] node->data;
    node->setLength(0);
    node->data = 0;
    return true;
}

FileNode* RamFS::createFile(DirectoryNode* parent, String name) {
    if (!isWritable())
        return 0;
    
    if (parent->getFS() != this)
        return 0;
    
    RamFileNode* node = new RamFileNode(name, this, parent);
    parent->loadContent();
    parent->getChildren().push(node);
    
    return node;
}

DirectoryNode* RamFS::createDirectory(DirectoryNode* parent, String name) {
    if (!isWritable())
        return 0;
    
    if (parent->getFS() != this)
        return 0;
    
    DirectoryNode* dir = new DirectoryNode(name, this, parent);
    parent->loadContent();
    parent->getChildren().push(dir);
    
    return dir;
}

bool RamFS::remove(DirectoryNode*, FSNode* node) {
    if (node->type() == FSNode::FS_DIRECTORY) {
        if (!((DirectoryNode *)node)->getChildren().size())
            return true;
        return false;
    } else if (node->type() == FSNode::FS_FILE) {
        char* d = ((RamFileNode *)node)->data;
        if (d)
            delete[] d;
        
        ((RamFileNode *)node)->data = 0;
        return true;
    }
    return false;
}