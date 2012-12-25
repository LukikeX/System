#ifndef FW_VT_DEFS_H
#define FW_VT_DEFS_H

#define MVT_ESC "\032"

#define MVTCMD_CLEAR      0x100
#define MVTCMD_SCROLL     0x101
#define MVTCMD_SETCOLOR   0x110
#define MVTCMD_SETBGCOLOR 0x111
#define MVTCMD_SETFGCOLOR 0x112
#define MVTCMD_MOVECSR    0x120
#define MVTCMD_SETCSRLIN  0x121
#define MVTCMD_SETCSRCOL  0x122
#define MVTCMD_HIDECSR    0x130
#define MVTCMD_SHOWCSR    0x131

struct mvtEscCmdT {
    int cmd;
    int a, b;
};

#endif