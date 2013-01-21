#include "VirtualTerminal.proto.h"
#include <TaskManager/Task.h>

void VirtualTerminal::keyPress(keyStatus ks) {
    if (kbdBufferEnd == kbdBufferStart - 1 || (kbdBufferEnd == 32 - 1 && !kbdBufferStart))
        return;
    
    kbdBuffer[kbdBufferEnd++] = ks;
    
    if (kbdBufferEnd == 32)
        kbdBufferEnd = 0;
}

keyStatus VirtualTerminal::getkeyPress(bool show, bool block) {
    keyboardMutex.waitLock();
    
    if (kbdBufferStart == kbdBufferEnd && !block) {
        keyboardMutex.unlock();
        return keyStatus();
    }
    
    while (kbdBufferStart == kbdBufferEnd)
        Task::currentThread()->sleep(10);
    
    keyStatus ret = kbdBuffer[kbdBufferStart++];
    
    if (kbdBufferStart == 32)
        kbdBufferStart = 0;
    
    keyboardMutex.unlock();
    
    if (show) {
        if (ret.hasChar && !ret.hasCmd)
            put(ret.character);
        else if (ret.hasCmd && !ret.hasChar && ret.command == KBDC_ENTER)
            put("\n");
        else if (ret.hasCmd && !ret.hasChar && ret.command == KBDC_TAB)
            put ("\t");
        else if (ret.hasCmd && !ret.hasChar && ret.command == KBDC_BACKSPACE)
            put("\b");
    }
    
    return ret;
}

String VirtualTerminal::readLine(bool show) {
    String ret = "";
    keyStatus tmp = getkeyPress(show);
    
    while (!(tmp.hasCmd && !tmp.hasChar && tmp.command == KBDC_ENTER)) {
        if (tmp.hasCmd && !tmp.hasChar && tmp.command == KBDC_BACKSPACE) {
            if (!ret.empty())
                ret = ret.substr(0, ret.size() - 1);
            else if (show)
                put (" ");
        } else if (tmp.hasCmd && !tmp.hasChar && tmp.command == KBDC_TAB)
            ret += "\t";
        else if (tmp.hasCmd && tmp.hasChar && tmp.modifiers == STATUS_CTRL && tmp.character == WChar("d")) {
            ret += EOF;
            break;
        } else if (tmp.hasChar && !tmp.hasCmd)
            ret += tmp.character;
        tmp = getkeyPress(show);
    }
    
    return ret;
}