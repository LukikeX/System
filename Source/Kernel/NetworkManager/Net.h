#ifndef NET_H
#define NET_H

#include <Core/Loader.h>

#define inetAddress(_a_, _b_, _c_, _d_) ((uint)( _a_ | (_b_ << 8) | (_c_ << 16) | (_d_ << 24))) 


class Net {
public:
    struct ifconfig_t {
        uint ipaddr;
        uint netmask;
        uint broadcast;
        uint mtu;
    };
};

#endif