#include "defs.h"
#include "item.h"
#include "../controller/controller.h"
#include "../rhs/rhs.h"
#include "../integrator/integrator.h"
#include <sstream>
#include <vector>
#include <iostream>
#include "comp_funcs.h"
/*
 * This sets the value of object passed equal to the 
 * address of the current item
 *
 * This is used for classes such as integrator, controller, etc
 * which will comprise most of the item types here.
 * This function can be rewritten for native types such as ints, etc
 */
void item::retrieve(void* p){
    *((const item**)p) = this;
}
//item function
//!Returns dependencies of item, for which there are none
std::vector<std::string> item::dependencies()const{
    return std::vector<std::string>();
}
//!Standard print function
void item::print() const{
    std::cout << "A print function has not been implemented for variable type "
        << this->type() << ", called by variable " << this->name() << std::endl;
}
/*!
 * Sets the name of the variable
 * @param n Variable name
 */
void item::setname(const std::string n){
    _name = n;
}
/*!
 * Returns the name of the variable
 * @return name of the variable
 */
const std::string& item::name()const {
    return _name;
}

//!Deprecated
void real8::parse(std::string inval){
    std::stringstream convert(inval);
    convert >> value;
}

/*!
 * Stores the value in the address pointed to by inval
 * @param inval Address where the value of the real8 is stored
 */
void real8::retrieve(void* inval){
    *(double*)inval = value;
}

/*!
 * Returns the type of real8
 * @return Type of real8 class, which is "real8"
 */
std::string real8::type() const{
    return "real8";
}

/*!
 * Returns dependencies for real8 class
 * @return Dependencies of real8 type, for which there are none
 */
std::vector<std::string> real8::dependencies() const{
    std::vector<std::string> w;
    return w;
}
/*!
 *Prints the value, along with the name
 */
void real8::print() const{
    std::cout<<this->type()<<" "<<this->name()<<"="<<value<<std::endl;
}


//string functions

/*
 * Parse the string input, deprecated
 */
void string::parse(std::string inval){
    value = inval;
}
/*!
 * Sets the value stored at inval equal to the string
 * @param inval The address of the string
 */
void string::retrieve(void* inval){
    *(std::string*)inval = value;
}

/*!
 * Returns the name of the string type, "string"
 * @return Name of string type, "string"
 */
std::string string::type() const {
    return "string";
}
/*!
 * Returns the list of dependencies for the string type, for which there are none
 * @return vector containging dependencies of string type, for which there are none
 */
std::vector<std::string> string::dependencies() const {
    std::vector<std::string> w;
    return w;
}
/*!
 * Prints the string
 */
void string::print() const{
    std::cout<<this->type()<<" "<<this->name()<<"="<<value<<std::endl;
}


//integer functions
//
/*!
 * Parses the input int, deprecated
 */
void integer::parse(std::string inval){
    std::stringstream convert(inval);
    convert >> value;
}
/*!
 * Prints the int
 */
void integer::print() const{
    std::cout << this->type()<<" "<<this->name() << "=" << value << std::endl;
}

/*!
 * Returns the name of the integer type
 * @return Name of the integer type, "integer"
 */
std::string integer::type() const{
    return "integer";
}

/*!
 * Returns the dependencies of the integer type, for which there are none
 */
std::vector<std::string> integer::dependencies() const{
    return std::vector<std::string>();
}

/*!
 * Sets the input pointer to the value of the integer
 */
void integer::retrieve(void* p){
    int* qq = (int*)p;
    *qq = value;
}
/*
 * Postprocesses the item_dim class, which sets the dimension to the input variable dimension
 */
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

/*!
 * Returns the dependencies of item_dim
 * \note
 * item_dim strongly depends on the input value integer dimension
 * @return Vector containing the dependencies of item_dim
 */
std::vector<std::string> item_dim::dependencies()const{
    std::vector<std::string> de;
    de.push_back("dimension");
    return de;
}


//variable functions

/*!
 * This function prints the value of the current variable
 */
void variable::print() const{
    std::cout << "variable " << this->name() << "=" << value;
}

/*!
 * This function assigns the real value stored to the value pointed
 * at by inval. It also keeps track of the pointer and updates it whenever
 * the variable has changed
 */
void variable::retrieve(void* inval){
    double* d = (double*)inval;
    *d = value;
    modifiers.remove(d);
    modifiers.push_back(d);
}

/*!
 * This function increments the variable.
 * The list of pointers will be managed by the engine class,
 * but for now it is unsafe. Currently the goal is to get a
 * functioning automated toroidal search, and move on from there
 */
void variable::inc(){
    value += inc_size;
    //deal with periodicity here
    if(value > high_bound){
        value = low_bound + (value-high_bound);
    }
    if(value < low_bound){
        value = high_bound - (low_bound-value);
    }
    std::list<double*>::iterator it = modifiers.begin();
    for(; it!= modifiers.end(); it++){
        (*(*it)) = value;
    }
}

/*!
 * This function copies the value without linking it to the actual variable
 * @param inval The pointer ot the double that is updated
 */
void variable::copy(double* inval){
    *inval = value;
}

//! Deprecated, won't be around too long
void variable::parse(std::string inval){
    real8::parse(inval);
    inc_size=value;
    low_bound=0;
    high_bound=6.2382;
    value=0;
}
/*!
 * This function create an item of the type name
 * @param name Name of the item to be created
 * @return Pointer to the created item
 */
item* item::create(std::string name, engineimp* in){
    item* rval=0;
    if(name == "real8"){
        rval = new real8();
        rval->holder=in;
        return rval;
    }
    if(name == "string"){
        rval = new string();
        rval->holder = in;
        return rval;
    }
    if(name == "integer"){
        rval = new integer();
        rval->holder=in;
        return rval;
    }
    if(name == "var"){
        rval = new variable();
        rval->holder=in;
        return rval;
    }
    rval=rhs::create(name);
    if(rval){
        rval->holder=in;
        return rval;
    }
    rval=integrator::create(name);
    if(rval){
        rval->holder=in;
        return rval;
    }
    rval=controller::create(name);
    if(rval){
        rval->holder=in;
        return rval;
    }
    name.append(" does not name a valid type");
    err(name, "item::create", "parser/item.cpp", FATAL_ERROR);
    //should really have an exit type code here
    return 0;
}

