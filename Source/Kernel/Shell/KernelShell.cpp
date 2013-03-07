#include "KernelShell.h"
#include <VTManager/ScrollableVT.h>
#include <DeviceManager/Keyboard.h>
#include <TaskManager/Task.h>
#include <Core/System.h>
#include <VFS/VFS.h>

ushort KernelShell::instances = 0;

ulong shellRun(void* ks) {
    KernelShell* shell = (KernelShell *)ks;
    uint ret = shell->run();
    while (true);
    
    delete shell;
    return ret;
}

KernelShell::KernelShell(DirectoryNode* cwd) {
    ScrollableVT* vt = new ScrollableVT(15, 76, 200);
    vt->map(9);
    Keyboard::setFocus(vt);
    setup(cwd, vt);
}

KernelShell::KernelShell(DirectoryNode* cwd, VirtualTerminal* vt) {
    setup(cwd, vt);
}

KernelShell::~KernelShell() {
    delete vt;
    instances--;
}

void KernelShell::setup(DirectoryNode* cwd, VirtualTerminal* vt) {
    this->vt = vt;
    Task::currentProcess()->setInVT(vt);
    Task::currentProcess()->setOutVT(vt);
    this->cwd = cwd;
    *vt << "Welcome to System kernel shell!\n";

    thread = new Thread(shellRun, (void *)this, true);
    instances++;
}

ulong KernelShell::run() {
    struct {
        const char* name;
        void (KernelShell::*cmd)(Vector<String>& args);
    } commands[] = {
        {"dir", &KernelShell::dir},
        {"cd", &KernelShell::cd},
        {"pwd", &KernelShell::pwd},
        {"cat", &KernelShell::cat},
        {"mkdir", &KernelShell::mkdir},
        {"rm", &KernelShell::rm},
        
        {0, 0}
    };
    
    while (true) {
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
        *kvt << VFS::path(cwd);
        kvt->setColor(VirtualTerminal::Grey);
        *kvt << ">";
        kvt->setColor(VirtualTerminal::Grey);
        
        Vector<String> cmd = vt->readLine().split(" ");
        
        if (cmd[0] == "help") {
            *vt << "chod do pice!\n";
        } else if (cmd[0] == "reboot")
            System::reboot();
        else if (cmd[0] == "panic")
            panic("Kernel panic!");
        else if (cmd[0] == "exit") {
            if (cmd.size() == 1)
                return 0;
            else 
                return cmd[1].toInt();
        } else if (cmd[0] != "" || cmd.size() != 1) {
            bool found = false;
            for (uint i = 0; commands[i].name; i++) {
                if (cmd[0] == commands[i].name) {
                    found = true;
                    if (commands[i].cmd)
                        (this->*(commands[i].cmd))(cmd);
                    else
                        *vt << "This command is not enabled!\n";
                    break;
                }
            }
            
            if (!found)
                *vt << "Unknown command: " << cmd[0] << "\n";
        }
    }
    
    return 1337;
}