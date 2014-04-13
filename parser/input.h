#pragma once
#ifndef INPUT_H
#define INPUT_H
#include "item.h"
class input{
    std::map<std::string, std::shared_ptr<item> >& invals;
    std::map<std::string, std::string> re_mappings;
    public:
    input(std::map<std::string, std::shared_ptr<item> >& values);
    std::shared_ptr<item>& operator [](const std::string& inval);
    inline std::shared_ptr<item> operator [](const char* inval){
        return (*this)[std::string(inval)];
    }
};
#endif
