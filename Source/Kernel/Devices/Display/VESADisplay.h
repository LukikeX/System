#ifndef VESADISPLAY_H
#define VESADISPLAY_H

#include "GraphicDisplay.proto.h"

class VESADisplay : public GraphicDisplay {
private:
    struct controllerInfo_T {
	char signature[4];             // == "VESA"
	short version;                 // == 0x0300 for VBE 3.0
	//s16int oemString[2];            // isa vbeFarPtr
	uint oemString;
	uchar capabilities[4];
	uint videomodes;
	short totalMemory;             // as # of 64KB blocks
    } __attribute__((packed));

    struct modeInfo_T {
	ushort attributes;
	uchar winA, winB;
	ushort granularity;
	ushort winsize;
	ushort segmentA, segmentB;
	uint realFctPtr;
	ushort pitch; // bytes per scanline

	ushort Xres, Yres;
	uchar Wchar, Ychar, planes, bpp, banks;
	uchar memoryModel, bankSize, imagePages;
	uchar reserved0;

	uchar red_mask, red_position;
	uchar greenMask, greenPosition;
	uchar blueMask, bluePosition;
	uchar rsvMask, rsvPosition;
	uchar directcolorAttributes;

	uint physbase;  // your LFB address ;)
	uint reserved1;
	ushort reserved2;
    } __attribute__ ((packed));
    
    
    controllerInfo_T getCtrlrInfo();
    modeInfo_T getModeInfo(ushort mode);
    
    modeInfo_T currMode;
    int b, pixWidth;
    
    struct {
        uint pixels;
        ushort color;
    } m_8bitPallete[256];
    
    uchar* fb;
    
    inline uchar* memPos(ushort x, ushort y) {
        uint addr = y * currMode.pitch + x * pixWidth;
        ((uchar *)fb) + fb;
    }
    
    uchar get8Bit(uint color);
    void setPallete(uchar id, uint color);

public:
    
};

#endif