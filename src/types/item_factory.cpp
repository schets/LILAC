#include "item_factory.h"
#include <map>
#include "defs.hpp"
typedef std::function<item*()> create_fnc; 
std::map<std::string, create_fnc>* types=0;
void item_factory::register_type(const std::string& name, create_fnc creator){
    //this enforces that types will be made upon entry into the function
    static std::map<std::string, create_fnc> make_types;
    types = &make_types;
    if(types->count(name)){
        err(name + " has already been registered in the type system",
                "item_factory::register_type", "utils/item_factory.cpp",
                FATAL_ERROR);
    }
    types->insert(std::pair<std::string, create_fnc>(name, creator));
}
item* item_factory::create_item(const std::string& name){
    if(!types || !types->count(name)){
        err(std::string() + "The requested type " + name + " does not exist",
                "item_factory::create_item", "utils/item_factory.cpp",
                FATAL_ERROR);
    }
    return (*types)[name]();
}
