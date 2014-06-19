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
#include "example_integrator.h"
#include "types/type_register.hpp"
#include "types/type_constructor.hpp"
#include "item_heads.hpp"
#include "example_integrator_tmpl.hpp"
#include "comp_funcs.hpp"
//Note that this function serves as a proxy for the actual version, the template class
int example_integrator::integrate(ptr_passer u, double t0, double tf){
    return actual_int->integrate(u, t0, tf);
}

std::string example_integrator::type() const{
    //note that it does not return the template type,
    //just example_integrator so it can be instantiated
    return "example_integrator";
}

std::vector<std::string> example_integrator::dependencies() const{
    //rules for dependencies:
    //A name preceded with a ! is optional
    std::string deps[] = {"rval1", "!rval2", "unsigned_var", "something"};
    //always call the dependencies of the parent class
    return make_append(deps, integrator::dependencies());
}

void example_integrator::postprocess(input& dat){
    //always do the base class processing first
    integrator::postprocess(dat);
    //create the actual integrator with the type_constructor class
    //see type_constructor.hpp for details on this
    type_constructor<example_integrator_tmpl>::create(&actual_int, rh_val);
    //rh_val is the rhs pointer that is retrieved by the base class

    //construct the actual_int class
    actual_int->holder = holder;
    actual_int->postprocess(dat);
}

//simply return actual_int
const std::type_info& example_integrator::vtype() const{
    return actual_int->vtype();
}

template class type_register<example_integrator>;
