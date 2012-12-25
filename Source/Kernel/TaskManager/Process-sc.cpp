#include "Process.h"
#include "Thread.h"
#include "Task.h"
#include "SyscallManager/Res.h"

Process::callT Process::callTable[] = {
    CALL0(PRIF_EXIT,       &Process::exitSC),
    CALL0(PRIF_ARGC,       &Process::argcSC),
    CALL1(PRIF_ARGV,       &Process::argvSC),
    CALL0(PRIF_START,      &Process::startSC),
    CALL2(PRIF_ALLOCPAGES, &Process::allocPagesSC),
    CALL2(PRIF_FREEPAGES,  &Process::freePagesSC),
    CALL1(PRIF_AUTODELETE, &Process::autoDeleteSC),
    CALL1(PRIF_PUSHARG,    &Process::pushArgSC),
    CALL1(PRIF_SETOUTVT,   &Process::setOutVTSC),
    CALL1(PRIF_SETINVT,    &Process::setInVTSC),
    CALL0(PRIF_GETPID,     &Process::getPid),
    CALL0(PRIF_GETPPID,    &Process::getPpid),
    CALL0(PRIF_GETUID,     &Process::getUid),
    CALL0(0, 0)
};

ulong Process::scall(uint wat, ulong a, ulong, ulong, ulong) {
    if (wat == PRIF_SGETCPR)
        return Task::currentProcess()->resId();
    else if (wat == PRIF_SRUN) {
        String* e = (String *)a;
        Process* p = Process::run(*e, 1); //@todo: UID!!
        if (p)
            return p->resId();
    } else if (wat == PRIF_SWAIT) {
        Process* p = Res::get<Process>(a, PRIF_OBJTYPE);
        if (Task::currentProcess()->getPid() != p->ppid)
            return 0;
        
        while (p->state != FINISHED && !p->autoDelete)
            Task::currentThread()->sleep(20);
        
        if (p->autoDelete)
            return PRIF_E_AUTODEL;
        
        ulong ret = p->retval;
        delete p;
        return ret;
    }
    return (ulong)-1;
}

bool Process::accessible() {
    return true; //uid == User::uid...
}

ulong Process::exitSC() {
    if (Task::currentProcess() != this)
        return 1;
    Task::currentThreadExits(E_EXIT);
    return 0;
}

ulong Process::argcSC() {
    //if () // 	if (Usr::uid() == m_uid or ISROOT or ISPARENT) { @todo
    return args.size();
}

ulong Process::argvSC(ulong idx) {
    if (idx >= args.size())
        return (ulong)-1;
    //if () // 	if (Usr::uid() == m_uid or ISROOT or ISPARENT) {
    
    return args[idx].serialize();
}

ulong Process::startSC() {
    if (Task::currentProcess()->getPid() == ppid) {
        start();
        return 1;
    }
    return 0;
}

ulong Process::allocPagesSC(ulong pos, ulong end) {
    if (Task::currentProcess() != this)
        return 1;
    
    if (pos & 0xFFF)
        pos = (pos & 0xFFFFFFFFFFFFF000) + 0x1000;
    
    if (end & 0xFFF)
        end = (end & 0xFFFFFFFFFFFFF000) + 0x1000;
    
    if (end - 1 >= 0xFFFFFFFFC0000000)
        return 1;
    
    for (ulong i = pos; i < end; i += 0x1000)
        pageDir->allocFrame(i, true, true);
    
    return 0;
}

ulong Process::freePagesSC(ulong pos, ulong end) {
    if (Task::currentProcess() != this)
        return 1;
    
    if (pos & 0xFFF)
        pos = (pos & 0xFFFFFFFFFFFFF000) + 0x1000;
    
    if (end & 0xFFF)
        end = (end & 0xFFFFFFFFFFFFF000) + 0x1000;
    
    if (end - 1 >= 0xFFFFFFFFC0000000)
        return 1;
    
    for (ulong i = pos; i < end; i += 0x1000)
        pageDir->freeFrame(i);
    
    return 0;
}

ulong Process::autoDeleteSC(ulong d) {
    if (Task::currentProcess()->getPid() != ppid)
        return 2;
    
    autoDelete = d != 0;
    return (autoDelete ? 1 : 0);
}

ulong Process::pushArgSC(ulong arg) {
    String* a = (String *)arg;
    args.push(*a);
    return 0;
}

ulong Process::setOutVTSC(ulong vtid) {
    if (Task::currentProcess()->getPid() != ppid)
        return 0;
    
    VirtualTerminal* vt = Res::get<VirtualTerminal>(vtid, VTIF_OBJTYPE);
    if (vt)
        setOutVT(vt);
    return 1;
}

ulong Process::setInVTSC(ulong vtid) {
    if (Task::currentProcess()->getPid() != ppid)
        return 0;
    
    VirtualTerminal* vt = Res::get<VirtualTerminal>(vtid, VTIF_OBJTYPE);
    if (vt)
        setInVT(vt);
    return 1;
}