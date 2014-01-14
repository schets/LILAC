#include "item.h"
#include <sstream>
#include <vector>
void item::setname(const std::string n){
    _name = n;
}
const std::string& item::name()const {
    return _name;
}
void real::parse(const std::string& inval){
    std::stringstream convert(inval);
    convert >> value;
}
void real::retrieve(void* inval) const {
    *(double*)inval = value;
}
std::string real::type() const{
    return "real";
}
std::vector<std::string> real::dependencies() const{
    std::vector<std::string> w;
    return w;
}
void string::parse(const std::string& inval){
    value = inval;
}
void string::retrieve(void* inval)const {
    *(std::string*)inval = value;
}
std::string string::type() const {
    return "string";
}
std::vector<std::string> string::dependencies() const {
    std::vector<std::string> w;
    return w;
}
