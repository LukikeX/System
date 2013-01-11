#ifndef VFS_H
#define VFS_H

#include "FileSystem.proto.h"
#include <Library/Vector.h>

class VFS {
private:
    typedef FileSystem* (*mountCallback)(Partition* partition, DirectoryNode* mountpoint, bool rw);
    
    struct localFST {
        const char* name;
        mountCallback cb;
    };
    static localFST localFS[];
    
    static Vector<FileSystem *> fileSystems;
    static DirectoryNode* rootNode;
    VFS();
    
public:
    bool mount(DirectoryNode* path, ulong size);
    bool mount(DirectoryNode* path, String devClass, ulong devId, ulong partId, bool rw = false);
    bool unmount(FileSystem* fs);
    
    static DirectoryNode* getRootNode() { return rootNode; }
    static void registerFileSystem(FileSystem* fs);
    static void unregisterFileSystem(FileSystem* fs);
    static FSNode* find(const String& path, FSNode* start = 0);
    static FSNode* createFile(const String& path, FSNode* start = 0, bool vrfyperm = false);
    static FSNode* createDirectory(const String& path, FSNode* start = 0, bool vrfyperm = false);
    
    static bool remove(FSNode* node);
    static bool remove(const String& path, FSNode* start = 0);
    static String path(FSNode* node);
};

#endif