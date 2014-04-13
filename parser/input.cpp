#include "input.h"

input::input(std::map<std::string, std::shared_ptr<item> >& values):invals(values){}

std::shared_ptr<item>& input::operator [](const std::string& value){
    auto&& mit = re_mappings.find(value);
        if(mit != re_mappings.end()){
            return invals[mit->second];
        }
    return invals[value];
}
