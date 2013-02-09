#include "VESADisplay.h"
#include <TaskManager/V86/V86.h>
#include <Core/Panic.h>
#include <Core/IO.h>

inline ushort rgbTo15(uint color) {
    return (
            (((color >> 16 & 0xFF) / 8) << 10) |
            (((color >> 8 & 0xFF) / 8) << 5) |
            ((color & 0xFF) / 8));
}

inline uint rgbFrom15(uint color) {
    return (
        (((color >> 10 & 0x1F) * 8) << 16) |
        (((color >> 5 & 0x1F) * 8) << 8) |
        ((color & 0x1F) * 8));
    }

inline uint rgbTo16(uint color) {
    return (
        (((color >> 16 & 0xFF) / 8) << 11) |
        (((color >> 8 & 0xFF) / 4) << 5) |
        ((color & 0xFF) / 8));
}

inline uint rgbFrom16(uint color) {
    return (
        (((color >> 11 & 0x1F) * 8) << 16) |
        (((color >> 5 & 0x3F) * 4) << 8) |
        ((color & 0x1F) * 8));
}

VESADisplay::controllerInfo_T VESADisplay::getCtrlrInfo() {
    V86::map();
    controllerInfo_T* info = (controllerInfo_T *)V86::alloc(sizeof(controllerInfo_T));
    info->signature[0] = 'V';
    info->signature[1] = 'B';
    info->signature[2] = 'E';
    info->signature[3] = '2';
    info->videomodes = 0;
    
    V86Thread::regsT regs;
    regs.ax = 0x4F00;
    regs.es = LIN_SEG(info);
    regs.di = LIN_OFF(info);
    V86::biosInt(0x10, regs);
    
    if (regs.ax != 0x004F)
        panic("Error when detecting VESA modes!");
    
    if (info->signature[3] != 'A')
        panic("No VESA signature");
    
    return *info;
}

VESADisplay::modeInfo_T VESADisplay::getModeInfo(ushort mode) {
    V86::map();
    modeInfo_T* m = (modeInfo_T *)V86::alloc(sizeof(modeInfo_T));
    Memory::clear((uchar *)m, sizeof(modeInfo_T));
    
    V86Thread::regsT regs;
    regs.ax = 0x00004F01;
    regs.cx = mode;
    regs.es = LIN_SEG(m);
    regs.di = LIN_OFF(m);
    V86::biosInt(0x10, regs);
    return *m;
}

void VESADisplay::getModes(Vector<Display::modeT>& to) {
   // panic("test");
    /*controllerInfo_T info = getCtrlrInfo();
    
    ushort* modes = (ushort *)((((info.videomodes & 0xFFFF0000) >> 12) | (info.videomodes & 0x0000FFFF)) & 0xFFFFFFFF00000000);
    for (uint i = 0; i < 64; i++) {
        if (modes[i] == 0xFFFF)
            break;
        
        modeInfo_T mode = getModeInfo(modes[i]);        
        if ((mode.attributes & 0x90) != 0x90)
            continue;
        
        if (mode.memoryModel != 4 && mode.memoryModel != 6)
            continue;
        
        if (mode.bpp != 24 && mode.bpp != 16 && mode.bpp != 15 && mode.bpp != 8)
            continue;
        */
    
       
        Display::modeT m;
        m.device = this;
        m.textCols = currMode.Xres / 9;
        m.textRows = currMode.Yres / 16;
        //m.identifier = ];
        m.graphWidth = currMode.Xres;
        m.graphHeight = currMode.Yres;
        m.graphDepth = currMode.bpp;
        to.push(m);
    //}
}

bool VESADisplay::setMode(Display::modeT& mode) {
    if (mode.device != this)
        return false;
    
    currMode = getModeInfo(mode.identifier);
    V86Thread::regsT regs;
    
    regs.ax = 0x4F02;
    regs.bx = mode.identifier | 0x4000;
    V86::biosInt(0x10, regs);
    if (regs.ax & 0xFF00)
        return false;
    
    if (currMode.bpp == 8) {
        for (uint i = 0; i < 16; i++) {
            m_8bitPallete[i].pixels = 0;
            m_8bitPallete[i].color = rgbTo15(consoleColor[i]);
            setPallete(i, consoleColor[i]);
        }
        
        for (uint i = 0; i < 16; i++) {
            m_8bitPallete[i].pixels = 0;
            m_8bitPallete[i].color = 0;
        }
    }

    fb = (uchar *)0xFFFFFFFFF0000000;
    for (uint i = 0; i < (uint)(currMode.Yres * currMode.pitch); i += 0x1000)
        PhysMem::kernelPageDirectory->map(PhysMem::kernelPageDirectory->getPage((ulong)(fb + i), true), (currMode.physbase + i) / 0x1000, false, false);

    pixWidth = (currMode.bpp + 1) / 8;
    clear();
    return true;
}

void VESADisplay::unsetMode() {
    for (uint i = 0; i < (uint)(currMode.Yres * currMode.pitch); i += 0x1000) {
        PageDirectory::PTE* p = PhysMem::kernelPageDirectory->getPage((ulong)(fb + i), false);
        if (p)
            p->present = 0, p->address = 0;
    }
}

void VESADisplay::clear() {
    for (uint* i = (uint *)(memPos(0, 0)); i < (uint *)(memPos(currMode.Xres, 0)); i++)
        *i = 0;
}

void VESADisplay::setPallete(uchar id, uint color) {
    IO::outB(0x03C6, 0xFF);
    IO::outB(0x03C8, id);
    IO::outB(0x03C9, ((color >> 16) & 0xFF) / 4);
    IO::outB(0x03C9, ((color >> 8) & 0xFF) / 4);
    IO::outB(0x03C9, (color & 0xFF) / 4);
}

uchar VESADisplay::get8Bit(uint color) {
    ushort c = rgbTo15(color);
    c &= ~ 0x0C63;
    for (ushort i = 0; i < 256; i++) 
        if (m_8bitPallete[i].color == c)
            return i;
    
    for (ushort i = 16; i < 256; i++) {
        if (!m_8bitPallete[i].pixels) {
            m_8bitPallete[i].color = c;
            setPallete(i, rgbFrom15(c));
            return i;
        }
    }
    return 0;
}

void VESADisplay::putPixel(ushort x, ushort y, uint color) {
    //if (x >= currMode.Xres || y >= currMode.Yres)
     //   return;
    
    union {
        uchar* c;
        ushort* w;
        uint* d;
    } p = {memPos(x, y)};
    
    *p.d = (*p.d & 0xFF000000) | color;
    return;
    
    if (currMode.bpp == 24)
        *p.d = (*p.d & 0xFF000000) | color;
    else if (currMode.bpp == 15)
        *p.w = rgbTo15(color);
    else if (currMode.bpp == 16)
        *p.w = rgbTo16(color);
    else if (currMode.bpp == 8) {
        m_8bitPallete[*p.c].pixels--;
        *p.c = get8Bit(color);
        m_8bitPallete[*p.c].pixels++;
    }
}

uint VESADisplay::getPixel(ushort x, ushort y) {
    if (x >= currMode.Xres || y >= currMode.Yres)
        return 0;
    
    uint ret = 0;
    union {
        uchar* c;
        ushort* w;
        uint* d;
    } p = {memPos(x, y)};
    
    if (currMode.bpp == 24)
        ret = *p.d & 0x00FFFFFF;
    else if (currMode.bpp == 15)
        ret = rgbFrom15(*p.w);
    else if (currMode.bpp == 16)
        ret = rgbFrom16(*p.w);
    else if (currMode.bpp == 8)
        ret = rgbFrom15(m_8bitPallete[*p.c].color);
    
    return ret;
}

void VESADisplay::drawChar(ushort line, ushort col, WChar c, uchar color) {
    uchar ch = c.toAscii();
    if (!ch)
        return;
    
    ushort sx = col * 9;
    ushort sy = line * 16;
    uint fgcolor = 1, bgcolor = 0;
    
    if (currMode.bpp == 24) {
        fgcolor = consoleColor[color & 0xF];
        bgcolor = consoleColor[(color >> 4) & 0xF];
    } else if (currMode.bpp == 15) {
        fgcolor = rgbTo15(consoleColor[color & 0xF]);
        bgcolor = rgbTo15(consoleColor[(color >> 4) & 0xF]);
    } else if (currMode.bpp == 16) {
        fgcolor = rgbTo16(consoleColor[color & 0xF]);
        bgcolor = rgbTo16(consoleColor[(color >> 4) & 0xF]);
    } else if (currMode.bpp == 8) {
        fgcolor = color & 0xF;
        bgcolor = (color >> 4) & 0xF;
    }
    
    if (c == WChar(" ")) {
        uchar* p = memPos(sx, sy);
        for (uint y = 0; y < 9; y++) {
            if (pixWidth == 1)
                Memory::set((ushort *)p, (ushort)bgcolor, 0);
            else if (pixWidth == 2)
                Memory::set((ushort *)p, (ushort)bgcolor, 0);
            else if (pixWidth == 3) {
                for (uint i = 0; i < 9; i++) {
                    p[0] = bgcolor;
                    p[1] = bgcolor >> 8;
                    p[2] = bgcolor >> 16;
                    p += 3;
                }
                p -= 27;
            }
            p += currMode.pitch;
        }
        return;
    }
    
    int y = 0;
    for (uchar* p = memPos(sx, sy); p < memPos(sx, sy + 16); p += currMode.pitch) {
        union {
            uchar* c;
            ushort* w;
            uint* d;
        } pos = { p + (8 * pixWidth) };
        uchar pixs = 0x12;//consoleFont[ch][y];
        
        if (pixWidth == 3) {
            *pos.d = (*pos.d & 0xFF000000) | bgcolor;
            for (int x = 0; x < 8; x++) {
                pos.c -= pixWidth;
                *pos.w = (pixs & 1 ? fgcolor : bgcolor);
                pixs = pixs >> 1;
            }
        } else if (pixWidth == 2) {
            *pos.w = bgcolor;
            for (int x = 0; x < 8; x++) {
                pos.c -= pixWidth;
                *pos.w = (pixs & 1 ? fgcolor : bgcolor);
                pixs = pixs >> 1;
            }
        } else if (pixWidth == 1) {
            m_8bitPallete[*pos.c].pixels--;
            *pos.c = bgcolor;
            
            for (int x = 0; x < 8; x++) {
                pos.c--;
                m_8bitPallete[*pos.c].pixels--;
                *pos.c = (pixs & 1 ? fgcolor : bgcolor);
                pixs = pixs >> 1;
            }
        }
        y++;
    }
}

bool VESADisplay::textScroll(ushort line, ushort col, ushort height, ushort width, uchar color) {
    uchar* start = memPos(col * 9, line * 16);
    uint count = width * 9 * pixWidth;
    uint diff = 16 * currMode.pitch;
    putCrsBuff();
    
    for (int i = 0; i < (height - 1) * 16; i++) {
        Memory::copy(start, start + diff, count);
        start += currMode.pitch;
    }
    
    for (uint i = 0; i < width; i++) 
        drawChar(line + height - 1, col + i, " ", color);
    
    if (csrBuff.line == line + height - 1)
        csrBuff.line--;
    drawCsr();
    
    return true;
}