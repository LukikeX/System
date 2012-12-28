#include "File.h"
#include "VFS.h"

File::File() : Ressource(FLIF_OBJTYPE, callTable), file(0), valid(false), writable(false), position(0) { }

File::File(String fileName, uchar mode, FSNode* start) : 
        Ressource(FLIF_OBJTYPE, callTable), file(0), valid(false), writable(false), position(0) {
    open(fileName, mode, start);
}

bool File::open(String fileName, uchar mode, FSNode* start) {
    if (valid)
        return false;
    
    FSNode* node = VFS::find(fileName, start);
    if (!node) {
        if (mode == FM_READ)
            return false;
        
        node = VFS::createFile(fileName, start);
        if (!node)
            return false;
        
        node->setPermissions(0644);
    }
    
    if (node->type() != FSNode::FS_FILE)
        return false;
    
    file = (FileNode *)node;
    writable = mode != FM_READ;
    
    if (mode == FM_READ || mode == FM_REPLACE)
        position = 0;
    else if (mode == FM_TRUNCATE) {
        if (file->truncate())
            return false;
        position = 0;
    } else if (mode == FM_APPEND)
        position = file->getLength();
    
    if (writable)
        file->writers++;
    else
        file->readers++;
    
    valid = true;
    return true;
}

void File::close() {
    if (!valid)
        return;
    
    if (writable)
        file->writers--;
    else
        file->readers--;
   
    valid = false;
    file = 0;
    position = 0;
    writable = false;
}

uint File::read(uint length, char* data) {
    if (!valid)
        return 0;
    
    uint ret = file->read(position, length, data);
    position += ret;
    return ret;
}

bool File::write(uint length, char* data) {
    if (!valid)
        return false;
    
    if (!writable)
        return false;
    
    if (file->write(position, length, data)) {
        position += length;
        return true;
    }
    return false;
}

bool File::eof() const {
    if (!valid)
        return false;
    
    return position == file->getLength();
}