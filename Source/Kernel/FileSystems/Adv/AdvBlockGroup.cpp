#include "AdvBlockGroup.h"

int AdvBlockGroup::getBlock(int group) {
    return parent->sb->firstDataBlock + 1 + (group * sizeof(AdvBlockGroup)) / parent->blockSize();
}

bool AdvBlockGroup::read(int group) {
    return parent->readOff(getBlock(group) * parent->blockSize() + 
            (group * sizeof(AdvBlockGroup)) % parent->blockSize(), (char *)this, sizeof(AdvBlockGroup));
}

bool AdvBlockGroup::update(int group) {
    return parent->writeOff(getBlock(group) * parent->blockSize() + 
            (group * sizeof(AdvBlockGroup)) % parent->blockSize(), (char *)this, sizeof(AdvBlockGroup));
}