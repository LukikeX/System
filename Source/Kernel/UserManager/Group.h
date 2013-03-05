#ifndef GROUP_H
#define GROUP_H

#include <Library/String.h>
#include "UserManager.h"

class Group {
private:
    String name;
    uint gid;
    
    Group(String name, uint gid): name(name), gid(gid) { }
    
public:
    void setName(String name) { this->name = name; UserManager::save(); }
    String getName() { return name; }
    uint getGid() { return gid; }
};

#endif