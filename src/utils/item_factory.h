#pragma once
#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H
#include <string>
#include <functional>
#include <map>
class item;
template<class T> class type_register;
typedef std::function<item*()> create_fnc;
class item_factory{
    template<class U> friend class type_register;
    static void register_type(const std::string& name, const create_fnc& creator);
    public:
    static item* create_item(const std::string& inval);
};
#endif
