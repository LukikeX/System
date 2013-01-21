#include "Binary.proto.h"

Binary* (*Binary::loaders[])(File& file) = {
    0
};

Binary* Binary::load(File& file) {
    Binary* ret;
    ulong i = 0;
    
    while (loaders[i]) {
        file.seek(0, File::SM_BEGINNING);
        ret = loaders[i](file);
        
        if (ret)
            break;
        i++;
    }
    
    return ret;
}