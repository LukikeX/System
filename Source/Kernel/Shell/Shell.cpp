#include "Shell.h"
#include "VFS/VFS.h"
#include <Core/System.h>
#include <Core/Loader.h>

DirectoryNode* Shell::path;
Shell::cmd_T Shell::commands[] = {
    {"echo", &Shell::echo},
    {"show", &Shell::show},
    {"reboot", &Shell::reboot},
    {"set", &Shell::set},
    {"clear", &Shell::clear},
    
    {"cd", &Shell::cd},
    {"dir", &Shell::dir},
    {"mkdir", &Shell::mkdir},
    {"rm", &Shell::rm},
    {"edit", &Shell::edit},
    
    {0, 0}
};

Shell::Shell() {
    path = (DirectoryNode *)VFS::find("/");
}

void Shell::printMode() {
    kvt->setColor(VirtualTerminal::Grey);
    *kvt << "[ ";
    kvt->setColor(VirtualTerminal::Red);
    *kvt << "root";
    kvt->setColor(VirtualTerminal::Grey);
    *kvt << "@";
    kvt->setColor(VirtualTerminal::Green);
    *kvt << "System";
    kvt->setColor(VirtualTerminal::Grey);
    *kvt << " ]:";
    kvt->setColor(VirtualTerminal::Cyan);
    *kvt << VFS::path(path);
    kvt->setColor(VirtualTerminal::Grey);
    *kvt << ">";
    kvt->setColor(VirtualTerminal::Grey);
}

void Shell::echo(Vector<String>& cmd) {
    for (uint i = 0; i < cmd.size(); i++)
        *kvt << cmd[i] << " ";
    *kvt << "\n";
}

void Shell::show(Vector<String>& cmd) {
    
}

void Shell::reboot(Vector<String>& cmd) {
    System::reboot();
}

void Shell::set(Vector<String>& cmd) {
    
}

void Shell::clear(Vector<String>& cmd) {

}

void Shell::cd(Vector<String>& cmd) {
    if (cmd.size() < 1) {
        *kvt << "Syntax: cd <directory>\n";
        return;
    }
    
    FSNode *node = VFS::find(cmd[0], path);
    if (node && node->type() & (FSNode::FS_DIRECTORY | FSNode::FS_MOUNTPOINT))
        path = (DirectoryNode *) node;
    else
        *kvt << "Directory does not exist!\n";
}

void Shell::dir(Vector<String>& cmd) {
    FSNode *node = VFS::find((cmd.size() ? cmd[0] : "."), path);
    
    if (!(node->type() & (FSNode::FS_DIRECTORY | FSNode::FS_MOUNTPOINT))) {
        *kvt << "Object is not a directory!\n";
        return;
    }
    
    Vector<FSNode *> files = ((DirectoryNode *)node)->getChildren();
    for (uint i = 0; i < files.size(); i++) {
        
        //Write directory mode
        char mode;
        uint colorMode;
        
        switch (files[i]->type()) {
            case FSNode::FS_DIRECTORY:
                mode = 'd';
                colorMode = VirtualTerminal::Green; 
                break;
            case FSNode::FS_APPLICATION:
                mode = 'a';
                colorMode = VirtualTerminal::Cyan;
                break;
            case FSNode::FS_BLOCKDEVICE:
                mode = 'b';
                colorMode = VirtualTerminal::Yellow;
                break;
            case FSNode::FS_CHARDEVICE:
                mode = 'c';
                colorMode = VirtualTerminal::Yellow;
                break;
            case FSNode::FS_MOUNTPOINT:
                mode = 'm';
                colorMode = VirtualTerminal::Red;
                break;
            case FSNode::FS_PIPE:
                mode = 'p';
                colorMode = VirtualTerminal::Green;
                break;
            case FSNode::FS_SYMLINK:
                mode = 'l';
                colorMode = VirtualTerminal::Blue;
                break;
            default:
                mode = '-';
                colorMode = VirtualTerminal::Grey;
        }
        *kvt << mode;
        
        uint perm = files[i]->getPermissions();
        *kvt << (char)((perm >> 8) & 4 ? 'r' : '-');
        *kvt << ((perm >> 8) & 2 ? 'w' : '-');
        *kvt << ((perm >> 8) & 1 ? 'x' : '-');
        
        *kvt << ((perm >> 4) & 4 ? 'r' : '-');
        *kvt << ((perm >> 4) & 2 ? 'w' : '-');
        *kvt << ((perm >> 4) & 1 ? 'x' : '-');
        
        *kvt << (perm & 4 ? 'r' : '-');
        *kvt << (perm & 2 ? 'w' : '-');
        *kvt << (perm & 1 ? 'x' : '-');
        
        *kvt << "    " << (int)files[i]->getLength() << " root root" << "    ";
        
        kvt->setColor(colorMode);
        *kvt << files[i]->getName() << "\n";
        kvt->setColor(VirtualTerminal::Grey);
    }
}

void Shell::mkdir(Vector<String>& cmd) {
    if (cmd.size() < 1) {
        *kvt << "Syntax: mkdir <file 1> <file 2> <...>\n";
        return;
    }
    
    for (uint i = 0; i < cmd.size(); i++)
        VFS::createDirectory(cmd[i], path);
}

void Shell::rm(Vector<String>& cmd) {
    if (cmd.size() < 1) {
        *kvt << "Syntax: rm <file 1> <file 2> <...>\n";
        return;
    }
    
    for (uint i = 0; i < cmd.size(); i++) {
        FSNode *node = VFS::find(cmd[0], path);
        if (node) {
            if (VFS::remove(node))
                return;
        }
    }
    
    *kvt << "Cannot remove " << cmd[0] << "\n";
}

void Shell::edit(Vector<String>& cmd) {
    
}