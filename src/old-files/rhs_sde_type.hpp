#ifndef RHS_SDE_TYPE_HPP
#define RHS_SDE_TYPE_HPP
#include "rhs_sde.h"
template<typename T> class rhs_sde_type:public rhs_sde{
    public:
    const std::type_info& vtype() const final{
        return typeid(T);
    }
    virtual void initial_condition(ptr_passer in, size_t len){
        static_assert(std::is_default_constructible<T>::value,
                "Type needs to be default constructable in rhs_sde_type");
        T* vals = (T*)in;
        for(size_t i = 0; i < len; i++){
            vals[i] = T();
        }
    }
};

#endif
