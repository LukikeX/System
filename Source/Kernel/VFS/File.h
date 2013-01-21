#ifndef FILE_H
#define FILE_H

#include "FileNode.proto.h"
#include <SyscallManager/Ressource.h>

class File : public Ressource {
public:
    enum {
        FM_READ     = 0,  //Open for read, put cursor at beginning
        FM_TRUNCATE = 1,  //Open for write, truncating file before
        FM_APPEND   = 2,  //Open for write, put cursor at end
        FM_REPLACE  = 3	  //Open for write, put cursor at beginning
    };
    
    enum {
        SM_FORWARD   = 0, //Seek from actual position
        SM_BACKWARD  = 1, //Seek from actual position, backward
        SM_BEGINNING = 2, //Seek from start of file
        SM_END       = 3, //Seek from end of file
    };
    
protected:
    FileNode* file;
    bool valid, writable;
    ulong position;
    
public:
    File();
    File(String fileName, uchar mode = FM_READ, FSNode* start = 0);
    virtual ~File() { close(); }
    
    bool open(String fileName, uchar mode = FM_READ, FSNode* start = 0, bool vrfyperm = false);
    void close();
    bool isValid() const { return valid; }
    
    uint read(uint length, char* data);
    bool write(uint length, char* data);
    bool eof() const;
    void reset() { position = 0; }
    ulong getLength() const { return file->getLength(); }
    bool seek(ulong count, uchar mode);
    
    template <typename T>
    bool read(T* d) {
        return (read(sizeof(T), (uchar *)d) == sizeof(T));
    }
    
    template <typename T>
    bool write(T* d) {
        return write(sizeof(T), (uchar *)d);
    }
    
    //Syscalls:
    static ulong scall(uint wat, ulong a, ulong b, ulong, ulong);
    
private:
    static callT callTable[];
    bool accessible();
    ulong closeSC();
    ulong validSC();
    ulong readSC(ulong length, ulong ptr);
    ulong writeSC(ulong length, ulong ptr);
    ulong seekSC(ulong count, ulong mode);
    ulong positionSC();
    ulong lengthSC();
    ulong eofSC();
    ulong resetSC();
};

#endif