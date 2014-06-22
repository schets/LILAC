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
#include "ode.h"
#include "ode_tmpl.hpp"
#include "types/type_constructor.hpp"
#include "types/type_register.hpp"
template class type_register<ode>;
/*!
 * This function returns the type name of the ode class, "ode"
 */
std::string ode::type() const{
    return "ode";
}
/*!
 * Returns the variable type being used by the object,
 */
const std::type_info& ode::vtype() const{
    return actual->vtype();
}

/*!
 * Returns the dependencies of the ode class.
 * The ode class has the same dependencies as the simulation class, as well as:
 *      - double tf: Ending time of the integration
 *
 *      - integrator integrator: Type of interator being used, must be a type that inherits from integrator class
 *
 * And has optional parameters: 
 *      - double t0: Starting time of integration, default value of 0
 *
 *      - double tw: Starting time of writing, default value of t0
 *
 *      - double w_step: Interval to record data, default value of -1.
 *
 * If w_step<=0 (to machine precision) or tw>= tf, then no recording will be done
 * Only the final value will be saved
 */
std::vector<std::string> ode::dependencies()const{
    std::string deps[] = {"!t0", "!tw", "!w_step", "tf", "integrator"};
    return make_append(deps, simulation::dependencies());
}
void ode::postprocess(input& in){
    simulation::postprocess(in);
    //find way to redirect this to actual later on?
    in.retrieve(inter, "integrator", this);
    type_constructor<ode_tmpl>::create(&actual, inter);
    actual->setname(this->name() + "_actual");
    actual->holder = holder;
    actual->postprocess(in);
    add_as_parent(actual);
}
double ode::simulate(){
    return actual->simulate();
}
