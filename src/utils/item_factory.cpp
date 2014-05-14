#include "item_factory.h"
#include "engine/item.h"
#include <map>
#include <iostream>
#include "defs.hpp"
std::map<std::string, create_fnc>* types=0;
class types_initer{
    public:
        types_initer(){
            types = new std::map<std::string, create_fnc>();
        }
        ~types_initer(){
            delete types;
        }
};
void item_factory::register_type(const std::string& name, const create_fnc& creator){
    //this enforces that types will be made upon entry into the function
    static types_initer make_types;
    if(types->count(name)){
        err(name + "has already been registered in the type system",
                "item_factory::register_type", "utils/item_factory.cpp",
                FATAL_ERROR);
    }
    types->insert(std::pair<std::string, create_fnc>(name, creator));
}
item* item_factory::create_item(const std::string& name){
    if(!types->count(name) || (!types)){
        err(std::string() + "The requested type " + name + " does not exist",
                "item_factory::create_item", "utils/item_factory.cpp",
                FATAL_ERROR);
    }
    return (*types)[name]();
}
