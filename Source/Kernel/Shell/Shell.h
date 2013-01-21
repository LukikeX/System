#ifndef SHELL_H
#define SHELL_H

#include <Library/Vector.h>
#include <Library/String.h>
#include <VFS/DirectoryNode.h>

class Shell {
private:
    struct cmd_T {
        const char* name;
        void (Shell::*cwd)(Vector<String>& cmd);
    };
    
    static cmd_T commands[];
    DirectoryNode* path;
    
public:
    Shell();
    static void printMode();
    static void setPath(DirectoryNode* path);
    
    static void show(Vector<String>& cmd);
    static void reboot(Vector<String>& cmd);
    static void set(Vector<String>& cmd);
    static void clear(Vector<String>& cmd);
    
    static void cd(Vector<String>& cmd);
    static void dir(Vector<String>& cmd);
    static void mkdir(Vector<String>& cmd);
    static void rm(Vector<String>& cmd);
    static void edit(Vector<String>& cmd);
    
};

#endif