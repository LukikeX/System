#ifndef USER_H
#define USER_H

#include <Library/Vector.h>
#include "Group.h"

class User {
private:
    String userName, completeName, password;
    uint uid;
    Group* group;
    Vector<Group *> extraGroups;
    
    User(String userName, String completeName, String password, Group* group, String extraGroups, uint uid)
            : userName(userName), completeName(completeName), password(password), uid(uid), group(group) {
        Vector<String> eg = extraGroups.split(",");
        for (uint i = 0; i < eg.size(); i++) {
            Group* g = UserManager::group(eg[i].toInt());
            if (g)
                this->extraGroups.push(g);
        }
    }
    
public:
    String getUserName() const { return userName; }
    String getCompleteName() const { return completeName; }
    String getPassword() const { return password; }
    uint getUid() const { return uid; }
    Group* getGroup() const { return group; }
    
    inline bool isInGroup(uint gid) {
        if (group->getGid() == gid)
            return true;
       
        for (uint i = 0; i < extraGroups.size(); i++)
            if(extraGroups[i]->getGid() == gid)
                return true;
        return false;
    }
    
    inline bool isInGroup(String name) {
        if (group->getName() == name)
            return true;
        
        for (uint i = 0; i < extraGroups.size(); i++)
            if (extraGroups[i]->getName() == name)
                return true;
        return false;
    }
    
    inline bool isInGroup(Group* g) {
        if (g == group)
            return true;
        
        for (uint i = 0; i < extraGroups.size(); i++)
            if (extraGroups[i] == g)
                return true;
        return false;
    }
    
    inline bool verifyPassword(String password) {
        return password == this->password;
    }
    
    String getGroups() {
        String ret;
        for (uint i = 0; i < extraGroups.size(); i++) {
            if (!ret.empty())
                ret += ",";
            ret += String::number(extraGroups[i]->getGid());
        }
        return ret;
    }
    
    void setUserName(String userName) { this->userName = userName; UserManager::save(); }
    void setCompleteName(String completeName) { this->completeName = completeName; UserManager::save(); }
    void setPassword(String password) { this->password = password; UserManager::save(); }
    void setGroup(Group* group) { this->group = group; UserManager::save(); }
    void addGroup(uint gid) {
        Group* g = UserManager::group(gid);
        if (g && !isInGroup(g))
            extraGroups.push(g);
        UserManager::save();
    }
};

#endif