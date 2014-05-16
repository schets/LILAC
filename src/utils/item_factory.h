#pragma once
#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H
#include <string>
class item;
template<class T> class type_register;
class item_factory{
    typedef item* (*create_fnc)(); 
    //could just be two functions, but I only want type_register to have access to the register_type function
    //item factory may have more compilcated state later on as well
    template<class U> friend class type_register;
    static void register_type(const std::string& name, const create_fnc& creator);
    public:
    static item* create_item(const std::string& inval);
};
#endif
