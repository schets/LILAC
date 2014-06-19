/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
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
#include "native_item.hpp"

using __HIDER__::native_names;

constexpr char native_names::double_name[];
constexpr char native_names::float_name[];
constexpr char native_names::unsigned_name[];
constexpr char native_names::string_name[];
constexpr char native_names::integer_name[];

template class type_register<_double>;
template class type_register<_float>;
template class type_register<string>;
template class type_register<integer>;
template class type_register<_unsigned>;
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
void native_item::_retrieve(retrieve_wrapper&& inval, item* caller){
    err("Retrieve called on native item not overloading class",
            "native_item::_retrieve", "engine/item.cpp",
            FATAL_ERROR);
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

