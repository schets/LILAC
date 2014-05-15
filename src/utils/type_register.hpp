#pragma once
#ifndef TYPE_REGISTER_HPP
#define TYPE_REGISTER_HPP
#include <type_traits>
#include "item_factory.h"
#include "defs.hpp"
class item;
template<class T>
class type_register{
    //by declaring regis as volatile,
    //the compiler hopefully won't optimize away the useless expression in
    //the destructor
    static volatile bool regis;
    static bool init();
    static item* create();
    //seems pointless but forces instantiation
    //non virtual doesn't work
    virtual bool get_regis(){
        return type_register<T>::regis;
    }
};

template<class T>
volatile bool type_register<T>::regis=type_register<T>::init();

template<class T>
bool type_register<T>::init(){
    static_assert(std::is_base_of<item, T>::value, "type_register can only be used on classes that inherit from item");
    static_assert(std::is_base_of<type_register<T>, T>::value, "type_register can only be used on classes that inherit from it, \
            using themselves as the template argument (class derived : type_register<derived>");
    static_assert(!std::is_abstract<T>::value, "type_register cannot be used with an abstract class");
    //these conditions also assert that T defines a type function
    T temp;
    item_factory::register_type(temp.type(), type_register<T>::create);
    return true;
}

template<class T>
item* type_register<T>::create(){
    return new T();
}
#endif
