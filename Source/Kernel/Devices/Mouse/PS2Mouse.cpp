#include "PS2Mouse.h"

PS2Mouse::PS2Mouse() {
    no = 0;

    IO::cli();
    wait(1);
    IO::outB(0x64, 0xA8);
    wait(1);
    IO::outB(0x64, 0x20);
    wait(0);
    uchar status = IO::inB(0x60) | 2;
    wait(1);
    IO::outB(0x64, 0x60);
    wait(1);
    IO::outB(0x60, status);
    write(0xF6);
    read();
    write(0xF4);
    read();
    IO::sti();
    
    Device::requestIRQ(this, 12);
}

void PS2Mouse::wait(char type) {
    for (uint i = 0; i < 100000; i++) {
        if ((IO::inB(0x64) & (type ? 1 : 2)) == (type ? 1 : 0))
            return;
    }
}

void PS2Mouse::write(char str) {
    wait(1);
    IO::outB(0x64, 0xD4);
    wait(1);
    IO::outB(0x60, str);
}

char PS2Mouse::read() {
    wait(0);
    return IO::inB(0x60);
}

void PS2Mouse::IRQHandler(IDT::regs*) {
    if (no == 3)
        no = 0;
    
    char s = IO::inB(0x60);
    switch (no++) {
        case 0:
            Mouse::setBtns(s);
            break;
        case 1:
            Mouse::SetX(s);
            break;
        case 2:
            Mouse::setY(s);
            break;
    }
}