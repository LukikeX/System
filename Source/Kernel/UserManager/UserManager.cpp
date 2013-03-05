#include "UserManager.h"
#include "User.h"
#include "Group.h"
#include <TaskManager/Task.h>

List<Group>* UserManager::groups = 0;
List<User>* UserManager::users = 0;

void UserManager::load() {
    
}

void UserManager::save() {
    
}

User* UserManager::user(uint uid) {
    for (List<User>* iter = users; iter; iter = iter->next())
        if (iter->v().getUid() == uid)
            return &iter->v();
    return 0;
}

User* UserManager::user(String username) {
    for (List<User>* iter = users; iter; iter = iter->next())
        if (iter->v().getUserName() == username)
            return &iter->v();
    return 0;
}

User* UserManager::user() {
    return user(uid());
}

Group* UserManager::group(uint gid) {
    for (List<Group>* iter = groups; iter; iter = iter->next())
        if (iter->v().getGid() == gid)
            return &iter->v();
    return 0;
}

Group* UserManager::group(String name) {
    for (List<Group>* iter = groups; iter; iter = iter->next())
        if (iter->v().getName() == name)
            return &iter->v();
    return 0;
}

uint UserManager::uid() {
    return Task::currentProcess()->getUid();
}

uint UserManager::uid(String username) {
    User* u = user(username);
    return u ? u->getUid() : (uint)-1;
}