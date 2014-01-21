#include "item.h"
#include "item_imp.h"
#include "../rhs/rhs.h"
#include "../integrator/integrator.h"
#include <sstream>
#include <vector>
#include <iostream>
#include "comp_funcs.h"
//item functions
std::vector<std::string> item::dependencies()const{
    return std::vector<std::string>();
}
void item::print() const{
    std::cout << "A print function has not been implemented for variable type "
        << this->type() << ", called by variable " << this->name() << std::endl;
}
void item::setname(const std::string n){
    _name = n;
}
const std::string& item::name()const {
    return _name;
}

//class real functions
void real::parse(std::string inval){
    std::stringstream convert(inval);
    convert >> value;
}

void real::retrieve(void* inval){
    *(double*)inval = value;
}
std::string real::type() const{
    return "real";
}
std::vector<std::string> real::dependencies() const{
    std::vector<std::string> w;
    return w;
}
void real::print() const{
    std::cout<<this->type()<<" "<<this->name()<<"="<<value<<std::endl;
}


//string functions
void string::parse(std::string inval){
    value = inval;
}
void string::retrieve(void* inval){
    *(std::string*)inval = value;
}
std::string string::type() const {
    return "string";
}
std::vector<std::string> string::dependencies() const {
    std::vector<std::string> w;
    return w;
}
void string::print() const{
    std::cout<<this->type()<<" "<<this->name()<<"="<<value<<std::endl;
}


//test functions
std::vector<std::string> test::dependencies() const{
    const static std::string deps[]= {"a", "b", "qqq"};
    std::vector<std::string> retvec(deps, deps+3);
    return retvec;
}
std::string test::type() const{
    return "test";
}


//integer functions
void integer::parse(std::string inval){
    std::stringstream convert(inval);
    convert >> value;
}
void integer::print() const{
    std::cout << this->type()<<" "<<this->name() << "=" << value << std::endl;
}
std::string integer::type() const{
    return "integer";
}
std::vector<std::string> integer::dependencies() const{
    return std::vector<std::string>();
}
void integer::retrieve(void* p){
    int* qq = (int*)p;
    *qq = value;
}
//item_dim functions
void item_dim::postprocess(std::map<std::string, item*>& dat){
    int dimt;
    dat["dimension"]->retrieve(&dimt);
    if(dimt <= 0){
        std::string errmess = "dimension invalid, must be >= 0";
        err(errmess, "int_dim::postprocess", "item/item.cpp",
                dat["dimension"], FATAL_ERROR);
    }
    dimension = (size_t)dimt;
}
std::vector<std::string> item_dim::dependencies()const{
    std::vector<std::string> de;
    de.push_back("dimension");
    return de;
}
// creator function
item* item::create(std::string name){
    if(name == "real"){
        return new real();
    }
    else if(name == "string"){
        return new string();
    }
    else if(name == "test"){
        return new test();
    }
    else if(name == "integer"){
        return new integer();
    }
    else if(name=="CNLS"){
        return rhs::create(name);
    }
    else if(name=="rk4"){
        return integrator::create(name);
    }
    name.append(" does not name a valid type");
    err(name, "item::create", "parser/item.cpp", WARNING);
    //should really have an exit type code here
    return 0;
}

