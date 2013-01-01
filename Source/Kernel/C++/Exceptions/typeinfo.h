#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <Core/typedef.h>

namespace std {
    class type_info {
    public:
        virtual ~type_info();
        bool operator== (const type_info& other) const;
        bool operator!= (const type_info& other) const;
        bool before(const type_info& other) const;
        const char* name() const;
        type_info();
        
    private:
        type_info(const type_info& rhs);
        type_info& operator= (const type_info& rhs);
        const char* __type_name;
        
    public:
        virtual bool __is_pointer_p() const { return false; }
        virtual bool __is_function_p() const { return false; }
        
        //virtual bool __do_catch(const type_info* thrown_type, void **thrown_object, uint outer) const;
        //virtual bool __do_upcast() const { return false; }
    };
}

#endif