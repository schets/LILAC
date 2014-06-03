#include "item.h"
#include "../controller/controller.h"
#include "../rhs/rhs.h"
#include "../integrator/integrator.h"
#include "../simulation/simulation.h"
#include <sstream>
#include "comp_funcs.hpp"
#include "item_heads.hpp"
#include "types/item_factory.h"
#include "types/type_register.hpp"
template class type_register<_double>;

template class type_register<_float>;
template class type_register<string>;
template class type_register<integer>;
template class type_register<_unsigned>;
template class type_register<variable>;
template class type_register<ftest1>;
template class type_register<ftest2>;
template class type_register<ftest3>;
#ifdef ICC
//!helper function for variable class, since icc can't compare them using the stl...
bool operator < (const std::weak_ptr<item>& a, std::weak_ptr<item>& b){
	if(a.expired()){
		return false;
	}
	if(b.expired()){
		return true;
	}
	auto vala = a.lock();
	auto valb = b.lock();
	return vala.get() < valb.get();
}
#endif
item::~item(){}
item::item(){
    has_write_name=0;
    holder=0;
}
void item::postprocess(input& indat){
}
void item::set_write_name(std::string wname){
    has_write_name=1;
    _write_name=wname;
}
const std::string& item::write_name() const{
    if(has_write_name){
        return _write_name;
    }
    return _name;
}

std::vector<std::string> native_item::dependencies() const{
    return std::vector<std::string>();
}
void native_item::postprocess(input& inval){
}
/*!
 * This sets the value of object passed equal to the 
 * address of the current item
 *
 * This is used for classes such as integrator, controller, etc
 * which will comprise most of the item types here.
 * This function can be rewritten for native types such as ints, etc
 */
void item::_retrieve(retrieve_wrapper&& p, item* caller){
    p.check_and_get_type(this);
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
    err(errmess, "item::update", "engine/item.cpp", WARNING);
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

void _double::parse(const std::string& inval){
    std::stringstream convert(inval);
    convert >> value;
}

/*!
 * Stores the value in the address pointed to by inval
 * @param inval Address where the value of the real8 is stored
 */
void _double::_retrieve(retrieve_wrapper&& inval, item* caller){
    inval.check_and_get_type(typeid(double), &value);
}

/*!
 * Returns the type of real8
 * @return Type of real8 class, which is "real8"
 */
std::string _double::type() const{
    return "double";
}

/*!
 *Prints the value, along with the name
 */
void _double::print() const{
    std::cout<<this->type()<<" "<<this->name()<<"="<<value<<std::endl;
}

//_float functions
//
void _float::parse(const std::string& inval){
    std::stringstream convert(inval);
    convert >> value;
}

/*!
 * Stores the value in the address pointed to by inval
 * @param inval Address where the value of the real8 is stored
 */
void _float::_retrieve(retrieve_wrapper&& inval, item* caller){
    inval.check_and_get_type(typeid(float), &value);
}

/*!
 * Returns the type of real8
 * @return Type of real8 class, which is "real8"
 */
std::string _float::type() const{
    return "float";
}

/*!
 *Prints the value, along with the name
 */
void _float::print() const{
    std::cout<<this->type()<<" "<<this->name()<<"="<<value<<std::endl;
}

//string functions

void string::parse(const std::string& inval){
    value = inval;
}
/*!
 * Sets the value stored at inval equal to the string
 * @param inval The address of the string
 */
void string::_retrieve(retrieve_wrapper&& inval, item* caller){
    inval.check_and_get_type(typeid(std::string), &value);
}

/*!
 * Returns the name of the string type, "string"
 * @return Name of string type, "string"
 */
std::string string::type() const {
    return "string";
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
void integer::parse(const std::string& inval){
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
 * Sets the input pointer to the value of the integer
 */
void integer::_retrieve(retrieve_wrapper&& inval, item* caller){
    inval.check_and_get_type(typeid(int), &value);
}

//unisgned functions
//
/*!
 * Parses the input int, deprecated
 */
void _unsigned::parse(const std::string& inval){
    std::stringstream convert(inval);
    convert >> value;
}
/*!
 * Prints the int
 */
void _unsigned::print() const{
    std::cout << this->type()<<" "<<this->name() << "=" << value << std::endl;
}

/*!
 * Returns the name of the _unsigned type
 * @return Name of the _unsigned type, "unsigned"
 */
std::string _unsigned::type() const{
    return "unsigned";
}


/*!
 * Sets the input pointer to the value of the _unsigned
 */
void _unsigned::_retrieve(retrieve_wrapper&& inval, item* caller){
    inval.check_and_get_type(typeid(size_t), &value);
}



//variable functions


/*!
 * This function create an item of the type name
 * @param name Name of the item to be created
 * @return Pointer to the created item
 */
std::shared_ptr<item> item::create(std::string name, engineimp* in){
    item* rval=item_factory::create_item(name);
    rval->holder = in;
    return std::shared_ptr<item>(rval);
}

