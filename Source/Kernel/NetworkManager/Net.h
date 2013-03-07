#ifndef NET_H
#define NET_H

#include <Core/Loader.h>

#define inetAddress(_a_, _b_, _c_, _d_) ((uint)( _a_ | (_b_ << 8) | (_c_ << 16) | (_d_ << 24))) 


class Net {
public:
    union ipAddress_t {
        uint a;
        uchar q[4];
    };
    
    struct ifconfig_t {
        uint ipaddr;
        uint netmask;
        uint broadcast;
        uint mtu;
    };
    
    struct adapter_t {
        bool up;
        uint type;
        void* data;
        uint mtu;
        uint overhead;
        
        ipAddress_t ipaddr;
        ipAddress_t netmask;
        ipAddress_t broadcast;
        ipAddress_t gateway;
        void* packetCache;
        
        uint dropped;
        uint errors;
        uint recived;
    };
    
    void ifconfig(String device, ifconfig_t* conf, bool up);
};

#endif