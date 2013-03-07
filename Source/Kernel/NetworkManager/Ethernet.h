#ifndef ETHERNET_H
#define ETHERNET_H

#include "Net.h"
#include <Library/Vector.h>

 //#define NE2000_TYPE 1
#define ETHER_FRAME_IP          0x0800
#define ETHER_FRAME_ARP         0x0806
#define ETHER_FRAME_RARP        0x8035

#define ETHER_SEND              1
#define ETHER_RECEIVE           2

#define ETHER_ALEN              6
#define ETHER_HLEN              14
#define ETHER_ZLEN              60
#define ETHER_FRAME_LEN         1514
#define ETHER_MAX_MTU           (ETHER_FRAME_LEN - ETHER_HLEN)

#define COMMAND                 0
#define PAGESTART               1
#define PAGESTOP                2
#define BOUNDARY                3
#define TRANSMITSTATUS          4
#define TRANSMITPAGE            4
#define TRANSMITBYTECOUNT0      5
#define NCR                     5
#define TRANSMITBYTECOUNT1      6
#define INTERRUPTSTATUS         7
#define CURRENT                 7       /* in page 1 */
#define REMOTESTARTADDRESS0     8
#define CRDMA0                  8
#define REMOTESTARTADDRESS1     9
#define CRDMA1                  9
#define REMOTEBYTECOUNT0        10
#define REMOTEBYTECOUNT1        11
#define RECEIVESTATUS           12
#define RECEIVECONFIGURATION    12
#define TRANSMITCONFIGURATION   13
#define FAE_TALLY               13
#define DATACONFIGURATION       14
#define CRC_TALLY               14
#define INTERRUPTMASK           15
#define MISS_PKT_TALLY          15
#define IOPORT                  16
#define PSTART                  0x46
#define PSTOP                   0x80
#define TRANSMITBUFFER          0x40

#define rcr                     0       /* value for Recv config. reg */
#define tcr                     0       /* value for trans. config. reg */
#define dcr                     0x58    /* value for data config. reg */
#define imr                     0x7f /*0x4b*/    /* value for intr. mask reg */

/* Some generic ethernet register configurations. */
#define E8390_TX_IRQ_MASK 0xa /* For register EN0_ISR */
#define E8390_RX_IRQ_MASK 0x5
#define E8390_RXCONFIG    0x4 /* EN0_RXCR: broadcasts, no multicast,errors */
#define E8390_RXOFF       0x20  /* EN0_RXCR: Accept no packets */
#define E8390_TXCONFIG    0x00  /* EN0_TXCR: Normal transmit mode */
#define E8390_TXOFF       0x02  /* EN0_TXCR: Transmitter off */

/*  Register accessed at EN_CMD, the 8390 base addr.  */
#define E8390_STOP  0x01  /* Stop and reset the chip */
#define E8390_START 0x02  /* Start the chip, clear reset */
#define E8390_TRANS 0x04  /* Transmit a frame */
#define E8390_RREAD 0x08  /* Remote read */
#define E8390_RWRITE  0x10  /* Remote write  */
#define E8390_NODMA 0x20  /* Remote DMA */
#define E8390_PAGE0 0x00  /* Select page chip registers */
#define E8390_PAGE1 0x40  /* using the two high-order bits */
#define E8390_PAGE2 0x80  /* Page 3 is invalid. */

class Ethernet {
public:
    struct hwAddress_t {
        uchar h[6];
        
        void setHwAddress(uchar a, uchar b, uchar c, uchar d, uchar e, uchar f) { h[0] = a; h[1] = b; h[2] = c; h[3] = d; h[4] = e; h[5] = f; }
    };
    
    struct IORequest_t {
        uchar cmd;
        uchar complete;
        hwAddress_t raddr;
        ushort type;
        uint length;
        void* memory;
        void* extra;
        
        IORequest_t* next;
        IORequest_t* prev;
    };
    
    struct ethernet_t {
        uint type;
        bool lock;
        hwAddress_t macAddr;
        IORequest_t* ioFirst, *ioLast;
        Net::adapter_t* adapter;
    };
    
private:
    Vector<ethernet_t> devs;
    
public:
    Net::adapter_t* getAdapter(int idx);
    Net::adapter_t* getAdapterByName(String device);
};


#endif