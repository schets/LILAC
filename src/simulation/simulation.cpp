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
#include "simulation.h"
#include "utils/comp_funcs.hpp"
void simulation::postprocess(input& invals){
    item_dim::postprocess(invals);
    invals.retrieve(cont, "controller", this);
    invals.retrieve(obj, "objective", this);
    invals.retrieve(out_f_name, "!out_file", this);
}

/*!
 * This returns the dependencies for the simulation class.
 * The simulation class has the same dependencies as item_dim, as well as:
 *
 *      - controller controller: This object is the controller that controls the variables in the current simulation class
 *      - objective objective: This object is the objective function for the 
 * \sa item_dim::dependencies
 * @return Dependencies for system
 */
std::vector<std::string> simulation::dependencies() const{
    std::string sys_deps[]={"controller", "objective"};
    return make_append(sys_deps, item_dim::dependencies());
}

double simulation::score(){
    return 0;
}
