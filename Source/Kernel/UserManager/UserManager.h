#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <Library/String.h>
#include <Library/List.h>

#define INGRP(group) UserManager::user()->isInGroup(group)
#define ISROOT UserManager::user()->isInGroup("root")

class User;
class Group;

class UserManager {
private:
    static List<User>* users;
    static List<Group>* groups;
    
public:
    static void load();
    static void save();
    
    static User* user(uint uid);
    static User* user(String username);
    static User* user();
    
    static Group* group(uint gid);
    static Group* group(String name);
    
    static uint uid(String username);
    static uint uid();
};

#endif