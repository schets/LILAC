#pragma once
#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H
#include <string>
#include <functional>
class item;
template<class T> class type_register;
class item_factory{
    //could just be two functions, but I only want type_register to have access to the register_type function
    //item factory may have more compilcated state later on as well
    template<class U> friend class type_register;
    static void register_type(const std::string& name, std::function<item*()>);
    public:
    static item* create_item(const std::string& inval);
};
#endif
