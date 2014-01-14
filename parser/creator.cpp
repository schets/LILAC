#include "creator.h"
#include <iostream>
item* creator::create(const std::string& name){
    if(name == "real")
        return new real();
    else if(name == "string")
        return new string();
    else if(name=="rhs"){
    }
    std::cout << "Bad Type Input";
    //should really have an exit type code here
    return 0;
}

