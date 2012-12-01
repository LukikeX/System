#include "SB.h"
#include <DeviceManager/Display.h>

SimpleVT* SB::prog, *SB::debug;
uint SB::count, SB::inProg, SB::length;

SB::SB(uint count) {
    this->count = count;
    inProg = 0;
    prog = new SimpleVT(3, 80);
    debug = new SimpleVT(Display::textRows(), Display::textCols());
    
    length = Display::textCols() / 2 + 1;
    prog->map(Display::textRows() / 2 - 2, length / 2 - 1);
    debug->map(0, 0);
    
    *prog << "╔";
    for (uint i = 2; i < length; i++)
        *prog << "═";
    
    *prog << "╗\n║";
    for (uint i = 2; i < length; i++)
        *prog << " ";
    *prog << "║\n╚";
    
    for (uint i = 2; i < length; i++)
        *prog << "═";
    *prog << "╝";
}

void SB::progress(const String& str) {
    *debug << "  " << str;
    
    uint l = Display::textCols() - str.getLength() - 12;
    WChar s(" ");
    for (uint i = 0; i < l; i++)
        debug->put(s);
    
    prog->setColor(7 << 4);
    uint a = (double)inProg / (double)count * (double)length;
    uint b = (double)(inProg + 1) / (double)count * (double)length;
    
    for (uint i = a; i < b; i++)
        if (i != length - 1) 
            prog->putChar(1, (!i ? 1 : i), " ");
}

void SB::ok() {
    *debug << "[  ";
    debug->setColor(2);
    *debug << "OK";
    debug->setColor(7);
    *debug << "  ]\n";
    
    prog->setColor(2 << 4);
    uint a = (double)inProg++ / (double)count * (double)length;
    uint b = (double)inProg / (double)count * (double)length;
    
    for (uint i = a; i < b; i++)
        if (i != length - 1) 
            prog->putChar(1, (!i ? 1 : i), " ");
}

void SB::failed() {
    *debug << "[";
    debug->setColor(4);
    *debug << "FAILED";
    debug->setColor(7);
    *debug << "]\n";
    
    prog->setColor(4 << 4);
    uint a = (double)inProg++ / (double)count * (double)length;
    uint b = (double)inProg / (double)count * (double)length;
    
    for (uint i = a; i < b; i++)
        if (i != length - 1) 
            prog->putChar(1, (!i ? 1 : i), " ");
}