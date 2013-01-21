#ifndef BINARY_PROTO_H
#define BINARY_PROTO_H

#include <VFS/File.h>
#include <TaskManager/Thread.h>

class Binary {
private:
    static Binary* (*loaders[])(File& file);
    
public:
    static Binary* load(File& file);
    virtual ~Binary();
    
    virtual ThreadEntry toProcess(Process* p) = 0;
};

#endif