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
#include "engine/item.h"
#include "toroidal.h"
#include "comp_funcs.hpp"
#include "types/type_register.hpp"
#include "engine/engineimp.h"
#include <algorithm>
template class type_register<toroidal>;
/*
 * This function returns the dependencies of the toroidal class
 * This class has the same dependencies as the controller class, along with
 *
 *      - int iterations: The number of iterations the controller will perform
 *      - double initial_inc: The stepsize of the first variable
 *      - double mul_fac: The multiplying factor to find the increment of the next variable, inc2=mul_fac*inc1, inc3=mul_fac*inc2, etc.
 *
 *\sa controller::dependencies, item_dim::dependencies
 */
std::vector<std::string> toroidal::dependencies() const{
    std::string deps[] = {"iterations", "initial_inc", "mul_fac"};
    return make_append(deps, controller::dependencies());
}


/*!
 * Performs the postprocessing for toroidal,
 * which is setting the number of iterations that are to be performed
 * @param dat Map containing the input values
 * \sa controller::postprocess, item_dim::postprocess
 */
void toroidal::postprocess(input& dat){
    controller::postprocess(dat);
    num_int=0;
    int _iterations;



    dat.retrieve(_iterations, "iterations", this);
    dat.retrieve(initial_inc, "initial_inc", this);
    dat.retrieve(mul_fac, "mul_fac", this);


    iterations = _iterations;
    if(mul_fac==0){
        err("Multiply factor, mul_fac, must not be equal to zero", 
               "toroidal::postprocess", "controller/toroidal.cpp",
               FATAL_ERROR);
    }
    if(initial_inc==0){
        err("The initial increment, initial_inc, must not be equal to zero", 
               "toroidal::postprocess", "controller/toroidal.cpp",
               FATAL_ERROR);
    }
    //temporary hack to help with c_elegans
    holder->index = index*iterations;
    //find the controllers place in the number of iterations
}

/*!
 * Returns the type of the class toroidal
 * @return Returns the type of the toroidal class, "toroidal"
 */
std::string toroidal::type() const{
	return "toroidal";
}

void toroidal::control(comp* u, objective* obj){
	double curinc = initial_inc;
	num_int++;
	num_cont++;
	if(vars.size() == 4){
		//hard coded to test against matlab code
		vars[0].lock()->inc(sqrt(0.11)*100.0*PI/1000.0);
		vars[1].lock()->inc(sqrt(0.13)*100.0*PI/1000.0);
		vars[2].lock()->inc(sqrt(0.17)*100.0*PI/1000.0);
		vars[3].lock()->inc(sqrt(0.19)*100.0*PI/1000.0);
		return;
	}
	vars.erase(std::remove_if(vars.begin(), vars.end(), [](std::weak_ptr<variable>& inval){
				return inval.expired();
				}), vars.end());

	//still resume loack here in case a multithreaded engine appears in the future
	for(auto _cvar : vars){
		std::shared_ptr<variable> cvar = _cvar.lock();
		if(cvar){
			cvar->inc(curinc);
			curinc *= mul_fac;
		}
	}
}

//!Adds a variable to the controller
void toroidal::addvar(std::weak_ptr<variable> v){
	vars.push_back(v);
}
//!Returns whether the controller is done or not
char toroidal::is_good(){
	return num_int < iterations;
}
//!Moves the controller through fake iterations if the starting index is greater than 1
int toroidal::pre_set(){
	for(size_t i = 0; i < index*iterations; i++){
		double curinc=initial_inc;
		for(auto _val : vars){
			std::shared_ptr<variable> var = _val.lock();
			if(var.use_count()){
				var->inc(curinc);
				curinc *= mul_fac;
			}
		}
	}
	return index*iterations;
}
