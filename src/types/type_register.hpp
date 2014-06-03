#pragma once
#ifndef TYPE_REGISTER_HPP
#define TYPE_REGISTER_HPP
#include <type_traits>
#include <string>
#include <iostream>
#include "item_factory.h"
#include "defs.hpp"
class item;

//!overloadable template to construct a basic object
/*
 * This function allows customizable construction code for use in type_register.
 * The pointer it returns is owned by the type_register functionality
 *
 * type_register will fail if a specialization if this function doesn't return an item*,
 * and if it returns a pointer that can't dynamic_cast to T*
 */
template<class T>
item* __maker__(){
    return new T();
}
//!Registers a string to a type creation function
/*
 * This class serves to force registration in item creator at program start
 * An instantiation of type_register<T> will register T into the item_factory class,
 * such that a call to item_factory::create_item(T::type())
 * will return a pointer to a default constructed T
 * \sa item_factory
 */
template<class T>
class type_register final_def {
    static bool regis;
    static bool init();
    static item* create();
    type_register(){};
};

template<class T>
bool type_register<T>::regis=type_register<T>::init();

template<class T>
bool type_register<T>::init(){
    //initializes error code so error code doesn't fail
    std::ios_base::Init init_output;
    static bool has_been_called=false;
    static_assert(std::is_base_of<item, T>::value, "type_register can only be used on classes that inherit from item");
    static_assert(!std::is_abstract<T>::value, "type_register cannot be used with an abstract class");
    //these conditions also assert that T defines a type function
    //
    //
    //strictly, static_cast works since we have already established that T is a base of item,
    //but a re-implementation of maker<T> may remove this behavior
    T* temp = dynamic_cast<T*>(__maker__<T>());
    if(temp){
        if(has_been_called){
            //redundant check but can't hurt, especially if code changes in the future
            err(std::string() + "Type register has already registered type " + temp->type(),
                    "type_register::init", "utils/type_register.hpp", FATAL_ERROR);
        }
        has_been_called=true;
        item_factory::register_type(temp->type(), __maker__<T>);
        delete temp;
    }
    else{
        err(std::string() + "type_register could not allocate a type of " + typeid(T).name(),
                    "type_register::init", "utils/type_register.hpp", FATAL_ERROR);
    }

    return true;
}

template<class T>
item* type_register<T>::create(){
    return new T();
}
#endif
