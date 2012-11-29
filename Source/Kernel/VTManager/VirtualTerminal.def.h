#ifndef VIRTUALTERMINAL_DEF_H
#define VIRTUALTERMINAL_DEF_H

#define MVT_ESC "\032"

struct mvtEscCmdT {
    int cmd;
    int a, b;
};

#endif