#include "SB.h"
#include <DeviceManager/Display.h>

SimpleVT* SB::prog, *SB::debug, *SB::progMsg, *SB::logo;
uint SB::count, SB::inProg, SB::length;

SB::SB(uint count) {
    this->count = count;
    inProg = 0;
    length = Display::textCols() / 2 + 1;
    
    prog = new SimpleVT(3, 80);
    progMsg = new SimpleVT(1, Display::textCols());
    debug = new SimpleVT(Display::textRows(), Display::textCols());
    
    if (false) //debug
        debug->map(0, 0);
    else {
        prog->map(Display::textRows() / 2 + 4, length / 2 - 1);
        progMsg->map(Display::textRows() / 2 + 8, 0);
        
        logo = new SimpleVT(8, Display::textCols());
        logo->map(2, 0);
        *logo << "\
             ######  ##    ##  ######  ######## ######## ##     ##              \
            ##    ##  ##  ##  ##    ##    ##    ##       ###   ###              \
            ##         ####   ##          ##    ##       #### ####              \
             ######     ##     ######     ##    ######   ## ### ##              \
                  ##    ##          ##    ##    ##       ##     ##              \
            ##    ##    ##    ##    ##    ##    ##       ##     ##              \
             ######     ##     ######     ##    ######## ##     ##              ";
    }
    
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

SB::~SB() {
    debug->unmap();
    prog->unmap();
    progMsg->unmap();
    logo->unmap();
    
    delete debug;
    delete prog;
    delete progMsg;
    delete logo;
}

void SB::progress(const String& str) {
    *debug << "  " << str;
    
    progMsg->put('\n', false);
    for (uint i = 0; i < (Display::textCols() - str.getLength()) / 2; i++)
        progMsg->put(' ', false);
    progMsg->write(str, false);
    
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
    
    //for (uint i = 0; i < 10000000; i++); //slow
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