#include "Shell.h"
#include "VFS/VFS.h"
#include <Core/System.h>
#include <Core/Loader.h>

Shell::cmd_T Shell::commands[] = {
    {"show", &Shell::show},
    {"reboot", &Shell::reboot},
    {"set", &Shell::set},
    {"clear", &Shell::clear},
    
    
    {"set", &Shell::set},
    {"set", &Shell::set},
    {"set", &Shell::set},
};

Shell::Shell() {
    path = VFS::find("/");
}

void Shell::printMode() {
/*    *kvt <<  << "[ ";
    *kvt << OStream::Red   << "root";
    *kvt << OStream::Grey << "@";
    *kvt << OStream::Green << "System";
    *kvt << OStream::Grey << " ]:";
    *kvt << OStream::Cyan  << vfs->Path(path);
    *kvt << OStream::Grey << ">";*/
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