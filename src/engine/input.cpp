#include "input.h"

input::input(std::map<std::string, item_wrapper >& values):invals(&values){}

item_wrapper& input::operator [](const std::string& value){
    auto&& d_it = re_directions.find(value);
    if(d_it != re_directions.end()){
        return d_it->second;
    }
    auto&& m_it = re_mappings.find(value);
    if(m_it != re_mappings.end()){
        return (*this)[m_it->second];
    }
    return (*invals)[value];
}

void input::add_redir(const std::string& name, const item_wrapper& inv){
    re_directions[name] = inv;
}

void input::add_remap(const std::string& newname, const std::string& oldname){
    re_mappings[oldname] = newname;
}
