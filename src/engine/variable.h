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
#ifndef VARIABLE_H
#define VARIABLE_H
#include "item.h"
#include "native_item.hpp"
//!stores a real number that is varied during the simulation
/*!
 * This class stores a real number that is allowed to vary during the simulation
 * It stores all of the values that are linked to it and updates them upon the change
 * A method of setting a variable that does not involve linking the memory is also implemented
 * This function does not re-implement type, as for the purposes of dependencies it is a real
 * If one must check for being an instance of variable, the check dynamic_cast operator must be used
 *
 * currently, all values are set to ge between 0 and 2*pi. Again, this is just to satisfy the CNLS
 * situation and have a function search. The engine uses the real parser and sets the initial value to zero
 * As the engine framework is further built, the incrementing/decrementing interface
 * will be generalized to be useful for general pruposes, not simply plain toroidal searches
 * \sa real
 */
class variable:public _double{
	//double* are not use by std since they are mostly to stack allocated mem
	std::map<item*, std::set<double*> > modifiers;
#ifdef ICC
	std::map<std::weak_ptr<item>, std::set<double*>> safe_mods;
#else
	std::map<std::weak_ptr<item>, std::set<double*>,
		std::owner_less<std::weak_ptr<item>>> safe_mods;
#endif
	public:
	double low_bound, high_bound, inc_size;
	virtual void print() const;
	virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    virtual void add_ref(double& in, std::shared_ptr<item> inval);
    virtual void add_ref(double& in, item* inval);
	virtual void copy(double* inval);
	virtual void parse(const std::string& inval);
	virtual std::string type() const;
	virtual void inc();
	void inc(double i);
	void set(double v);
};

#endif
