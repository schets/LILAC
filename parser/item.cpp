#include "defs.h"
#include "item.h"
#include "../controller/controller.h"
#include "../rhs/rhs.h"
#include "../integrator/integrator.h"
#include "../simulation/simulation.h"
#include <sstream>
#include <vector>
#include <iostream>
#include "comp_funcs.h"
#include "variable.cpp"
/*!
 * This sets the value of object passed equal to the 
 * address of the current item
 *
 * This is used for classes such as integrator, controller, etc
 * which will comprise most of the item types here.
 * This function can be rewritten for native types such as ints, etc
 */
void item::retrieve(void* p, item* caller){
    *((const item**)p) = this;
}
/*!
 * Updates structures that are held in the class.
 * This is called after each iteration of the control algorithms
 * so that items can update internal parameters such as matrices
 * and whatnot. The controller updates the actual values for parameters
 * so if a class only depends on the direct values than the function need not be
 * re-implemented
 */
void item::update(){
    std::string errmess = this->name();
    errmess.append(" of type ");
    errmess.append(this->type());
    errmess.append(" does not support runtime updating");
    err(errmess, "item::update", "parser/item.cpp", WARNING);
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
void real8::retrieve(void* inval, item* caller){
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
void string::retrieve(void* inval, item* caller){
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
void integer::retrieve(void* p, item* caller){
    int* qq = (int*)p;
    *qq = value;
}
/*
 * Postprocesses the item_dim class, which sets the dimension to the input variable dimension
 */
void item_dim::postprocess(std::map<std::string, item*>& dat){
    int dimt;
    dat["dimension"]->retrieve(&dimt, this);
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
    rval=simulation::create(name);
    if(rval){
        rval->holder=in;
        return rval;
    }
    name.append(" does not name a valid type");
    err(name, "item::create", "parser/item.cpp", FATAL_ERROR);
    //should really have an exit type code here
    return 0;
}

