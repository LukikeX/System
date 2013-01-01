#include "typeinfo.h"

namespace std {
    type_info::~type_info() { }
    
    bool type_info::operator ==(const type_info& other) const {
        return __type_name == other.__type_name;
    }
    
    bool type_info::operator !=(const type_info& other) const {
        return __type_name != other.__type_name;
    }
    
    bool type_info::before(const type_info& other) const {
        return __type_name < other.__type_name;
    }
    
    const char* type_info::name() const {
        return __type_name;
    }
    
    type_info::type_info(const type_info& rhs) {
        __type_name = rhs.__type_name;
    }
    
    type_info& type_info::operator =(const type_info& rhs) {
        return *new type_info(rhs);
    }
}