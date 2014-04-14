#include "input.h"

input::input(std::map<std::string, item_wrapper >& values):invals(&values){}

item_wrapper& input::operator [](const std::string& value){
    auto&& d_it = re_directions.find(value);
    if(d_it != re_directions.end()){
        std::cout << value + " redirected to " + d_it->second->name() << std::endl;
        return d_it->second;
    }
    return (*invals)[value];
}

void input::add_redir(const std::string& name, const item_wrapper& inv){
    re_directions[name] = inv;
}
