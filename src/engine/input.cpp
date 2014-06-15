#include "input.h"

input::input(std::map<std::string, item_wrapper >& values):invals(&values){}

void input::insert_item(std::shared_ptr<item> inval){
    if(inval.use_count()){
        if(invals->count(inval->name())==0){
            invals->insert(std::pair<std::string, std::shared_ptr<item>>(
                        inval->name(), inval));
        }
        else{
            err(std::string("Item ") + inval->name() + " already exists in the engine",
                    "input::insert_item", "engine/input.cpp",
#ifdef DEBUG
                    FATAL_ERROR);
#else
                    WARNING);
#endif
        }
    }
    else{
        err("Null item inserted into input map", "input::insert_item",
                "engine/input.cpp", FATAL_ERROR);
    }
}

item_wrapper& input::get_val(const std::string& value){
    auto&& d_it = re_directions.find(value);
    if(d_it != re_directions.end()){
        return d_it->second;
    }
    auto&& m_it = re_mappings.find(value);
    if(m_it != re_mappings.end()){
        return get_val(m_it->second);
    }
    return (*invals)[value];
}

void input::add_redir(const std::string& name, const item_wrapper& inv){
    re_directions[name] = inv;
}

void input::add_remap(const std::string& newname, const std::string& oldname){
    re_mappings[oldname] = newname;
}
