#include "item.h"
#include <sstream>
void real::parse(const std::string& inval){
    std::stringstream convert(inval);
    convert >> value;
}
void integer::parse(const std::string& inval){
    std::stringstream convert(inval);
    convert >> value;
}
void real::retrieve(void* inval){
    *(double*)inval = value;
}
void integer::retrieve(void* inval){
    *(int*)inval = value;
}
void string::parse(const std::string& inval){
    value = inval;
}
void string::retrieve(void* inval){
    *(std::string*)inval = value;
}
