#pragma once
#ifndef PTR_PASSER_HPP
#define PTR_PASSER_HPP
#include "defs.hpp"
class ptr_passer{
    const std::type_info& tval;
    void* ptr;
    bool is_void;
    public:
    template<class T>
        ptr_passer(T* inptr):tval(typeid(T)), ptr(inptr), is_void(typeid(T) == typeid(void)){
        };
    ptr_passer(const ptr_passer& inval):tval(inval.tval), ptr(inval.ptr), is_void(inval.is_void){
    }
    template<class T>
        T* get(){
            if(!is_void && typeid(T) != tval){
                err(std::string("A pointer of type ") + typeid(T).name() +
                        " has been requested from a ptr_passer of type " + tval.name(),
                        "ptr_passer/get_ptr.hpp", "utils/ptr_passer.hpp", FATAL_ERROR);
            }
            return static_cast<T*>(ptr);
        }
};
#endif
