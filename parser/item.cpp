#include "item.h"
#include "item_imp.h"
#include "../rhs/rhs.h"
#include <sstream>
#include <vector>
#include <iostream>
void item::print(){
    std::cout << "A print function has not been implemented for variable type "
        << this->type() << std::endl;
}
void item::setname(const std::string n){
    _name = n;
}
const std::string& item::name()const {
    return _name;
}
void real::parse(std::string inval){
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
void string::parse(std::string inval){
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
std::vector<std::string> test::dependencies() const{
    const static std::string deps[]= {"a", "b", "qqq"};
    std::vector<std::string> retvec(deps, deps+3);
    return retvec;
}
std::string test::type() const{
    return "test";
}
item* item::create(const std::string name){
    if(name == "real")
        return new real();
    else if(name == "string")
        return new string();
    else if(name == "test"){
        return new test();
    }
    else if(name=="rhs"){
    }
    std::cout << "Bad Type Input";
    //should really have an exit type code here
    return 0;
}
