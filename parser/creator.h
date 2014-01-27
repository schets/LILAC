#ifndef CREATOR_H
#define CREATOR_H
#include "item.h"
#include <string>
//This class holds a map that asigns a type to each value
//and also initializaes an item for that type
class creator{
    public:
        static item* create(const std::string& name);
};
#endif
