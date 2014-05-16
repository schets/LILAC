#pragma once
#ifndef TYPE_REGISTER_HPP
#define TYPE_REGISTER_HPP
#include <type_traits>
#include <string>
#include "item_factory.h"
#include "defs.hpp"
class item;
template<class T>
class type_register final {
    static bool regis;
    static bool init();
    static item* create();
    type_register(){};
};

template<class T>
bool type_register<T>::regis=type_register<T>::init();

template<class T>
bool type_register<T>::init(){
    static bool has_been_called=false;
    static_assert(std::is_base_of<item, T>::value, "type_register can only be used on classes that inherit from item");
    static_assert(!std::is_abstract<T>::value, "type_register cannot be used with an abstract class");
    //these conditions also assert that T defines a type function
    T temp;
    if(has_been_called){
        //stdout/stderr seem to be not initialized before main, but oh well
        //redundant check but can't hurt, especially if code changes in the future
        err(std::string() + "Type register has already registered type " + temp.type(),
                "type_register::init", "utils/type_register.hpp", FATAL_ERROR);
    }
    has_been_called=true;
    item_factory::register_type(temp.type(), type_register<T>::create);
    return true;
}

template<class T>
item* type_register<T>::create(){
    return new T();
}
#endif
