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
#include "integrator.h"

#include "comp_funcs.hpp"
/*!
 * This function returns the dependencies of the integrator class
 * \note
 * Integrator has no explicit dependencies, returns item_dim::dependencies
 *
 * @return A vector containing the dependencies for integrator
 * \sa item_dim::dependencies
 */
std::vector<std::string> integrator::dependencies() const {
    std::string rhsval[] = {"rhs"};
    return make_append(rhsval, item_dim::dependencies());
}

//! Initiates the integrator from input values
/*!
 * This function initializes the internal memory and variables in the initiator class
 * When called, all parameters returned by dependencies are promised to have been processed
 * at the appropriate level of detail
 * \sa item_dim::postprocess
 */
void integrator::postprocess(input& dat){
    item_dim::postprocess(dat); 
    dat.retrieve(rh_val, "rhs", this);
}

void integrator::initial_condition(ptr_passer in, size_t len){
    rh_val->initial_condition(in, len);
}
