#include "Display.h"
#include "Device.h"
#include "VTManager/VT.h"
#include <Devices/Display/Display.proto.h>
#include <Devices/Display/VGATextOutput.h>

Vector<Display::modeT> Display::modes;
Display::modeT Display::mode;

void Display::putChar(ushort line, ushort col, WChar c, uchar color) {
    if (line >= mode.textRows || col >= mode.textCols)
        return;
    mode.device->putChar(line, col, c, color);
}

void Display::moveCursor(ushort line, ushort col) {
    if (line >= mode.textRows || col >= mode.textCols)
        return;
    mode.device->moveCursor(line, col);
}

bool Display::textScroll(ushort line, ushort col, ushort height, ushort width, uchar color) {
    return mode.device->textScroll(line, col, height, width, color);
}

void Display::getModes() {
   modes.clear();
    Vector<DeviceProto *> d = Device::findDevices("display");
    
    for (uint i = 0; i < d.size(); i++)
        ((DisplayProto *)(d[i]))->getModes(modes);
}

bool Display::setMode(modeT& cMode) {
    mode.device->unsetMode();
    
    if (cMode.device->setMode(cMode)) {
        mode = cMode;
        VT::redrawScreen();
        return true;
    }
    return false;
}

void Display::setText(VGATextoutput* out) {
    mode.device = out;
    out->clear();
    mode.textCols = 80;
    mode.textRows = 25;
}

void Display::clear() {
    mode.device->clear();
}