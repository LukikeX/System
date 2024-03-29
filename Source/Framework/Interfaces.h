#ifndef FW_INTERFACES_H
#define FW_INTERFACES_H

//==============================================================================
#define VTIF_OBJTYPE     0x100

#define VTIF_SGETPRINVT  0x01
#define VTIF_SGETPROUTVT 0x02
#define VTIF_WRITE       0x03
#define VTIF_PUT         0x04
#define VTIF_READLINE    0x05
#define VTIF_GETKEYPRESS 0x06
#define VTIF_ISBOXED     0x07
#define VTIF_GETHEIGHT   0x08
#define VTIF_GETWIDTH    0x09


//==============================================================================
#define PRIF_OBJTYPE     0x200

#define PRIF_SGETCPR     0x01
#define PRIF_SRUN        0x02
#define PRIF_SWAIT       0x03
#define PRIF_EXIT        0x04
#define PRIF_ALLOCPAGES  0x05
#define PRIF_FREEPAGES   0x06
#define PRIF_GETPID      0x07
#define PRIF_GETPPID     0x08
#define PRIF_GETUID      0x09
#define PRIF_ARGC        0x0A
#define PRIF_ARGV        0x0B
#define PRIF_START       0x0C
#define PRIF_AUTODELETE  0x0D
#define PRIF_PUSHARG     0x0E
#define PRIF_SETOUTVT    0x0F
#define PRIF_SETINVT     0x10

#define PRIF_E_AUTODEL   0x0FFFFF03

//#define PRIF_AUTHPW 0x40
//#define PRIF_AUTHNOPW 0x41


//==============================================================================
#define THIF_OBJTYPE     0x300

#define THIF_SGETCTH     0x01
#define THIF_SLEEP       0x02
#define THIF_FINISH      0x03


//==============================================================================
#define FLIF_OBJTYPE     0x400

#define FLIF_SOPEN       0x01
#define FLIF_CLOSE       0x05
#define FLIF_VALID       0x06
#define FLIF_READ        0x07
#define FLIF_WRITE       0x08
#define FLIF_POSITION    0x09
#define FLIF_LENGTH      0x0A
#define FLIF_EOF         0x0B
#define FLIF_SEEK        0x0C
#define FLIF_RESET       0x0D


//==============================================================================
#define FNIF_OBJTYPE     0x500

#define FNIF_SGETRFN     0x01
#define FNIF_SGETCWD     0x02
#define FNIF_SFIND       0x03
#define FNIF_SMKDIR      0x04
#define FNIF_GETNAME     0x05
#define FNIF_TYPE        0x06
#define FNIF_GETPARENT   0x07
#define FNIF_GETLENGTH   0x08
#define FNIF_GETUID      0x09
#define FNIF_GETGID      0x0A
#define FNIF_GETPERM     0x0B
#define FNIF_GETPATH     0x0C
#define FNIF_SETCWD      0x0D
#define FNIF_REMOVE      0x0E
#define FNIF_GETIDXCHILD 0x0F
#define FNIF_GETNCHILD   0x10


//==============================================================================
#define SYIF_OBJTYPE     0x600
#define SYIF_SREBOOT     0x01


#endif