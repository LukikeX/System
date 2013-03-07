#include "KernelShell.h"
#include <VFS/VFS.h>
#include <VFS/File.h>

void KernelShell::dir(Vector<String>& args) {
    FSNode *node = VFS::find((args.size() == 2 ? args[1] : "."), cwd);
    
    if (!(node->type() & (FSNode::FS_DIRECTORY | FSNode::FS_MOUNTPOINT))) {
        *vt << "Not a directory: " << args[1] << "\n";
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
        *vt << mode;
        
        uint perm = files[i]->getPermissions();
        *vt << (char)((perm >> 8) & 4 ? 'r' : '-');
        *vt << ((perm >> 8) & 2 ? 'w' : '-');
        *vt << ((perm >> 8) & 1 ? 'x' : '-');
        
        *vt << ((perm >> 4) & 4 ? 'r' : '-');
        *vt << ((perm >> 4) & 2 ? 'w' : '-');
        *vt << ((perm >> 4) & 1 ? 'x' : '-');
        
        *vt << (perm & 4 ? 'r' : '-');
        *vt << (perm & 2 ? 'w' : '-');
        *vt << (perm & 1 ? 'x' : '-');
        
        *vt << "    " << (int)files[i]->getLength() << " root root" << "    ";
        
        vt->setColor(colorMode);
        *vt << files[i]->getName() << "\n";
        vt->setColor(VirtualTerminal::Grey);
    }
}

void KernelShell::cd(Vector<String>& args) {
    if (args.size() == 1) {
        *vt << "Syntax: cd <directory>\n";
        return;
    }
    
    FSNode *node = VFS::find(args[1], cwd);
    if (node && node->type() & (FSNode::FS_DIRECTORY | FSNode::FS_MOUNTPOINT))
        cwd = (DirectoryNode *)node;
    else
        *vt << "Not a directory: " << args[1] << "\n";
}

void KernelShell::pwd(Vector<String>&) {
    *vt << VFS::path(cwd) << "\n";
}

void KernelShell::cat(Vector<String>& args) {
    if (args.size() == 1)
        *vt << "syntax: cat <file 1> <file 2> ... <file>\n";
    
    for (uint i = 1; i < args.size(); i++) {
        File f(args[i], File::FM_READ, cwd);
        if (f.isValid()) {
            char* buff = new char[f.getLength() + 1];
            f.read(f.getLength(), buff);
            buff[f.getLength()] = 0;
            
            *vt << String((const char *)buff);
            delete buff;
        }
    }
}

void KernelShell::mkdir(Vector<String>& args) {
    if (args.size() == 1) {
        *vt << "Syntax: mkdir <file 1> <file 2> ... <file>\n";
        return;
    }
    
    for (uint i = 1; i < args.size(); i++)
        if (!VFS::createDirectory(args[i], cwd))
            *vt << "Error while creating directory: " << args[i] << "\n";
}

void KernelShell::rm(Vector<String>& args) {
    if (args.size() == 1) {
        *vt << "Syntax: rm <file 1> <file 2> ... <file>\n";
        return;
    }
    
    for (uint i = 1; i < args.size(); i++) {
        if (!VFS::remove(args[i], cwd)) 
            *vt << "Error while removind file: " << args[i] << "\n";
    }
}