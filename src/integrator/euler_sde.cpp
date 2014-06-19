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
#include "../utils/comp_funcs.hpp"
#include "euler_sde.h"
#include "euler_sde_tmpl.hpp"
#include "types/type_constructor.hpp"
#include "types/type_register.hpp"
template class type_register<euler_sde>;

int euler_sde::integrate(ptr_passer u, double t0, double tf){
    return actual->integrate(u, t0, tf);
}
euler_sde::~euler_sde(){
}
std::vector<std::string> euler_sde::dependencies() const{
    std::string deps[] = {"stepsize", "calc_dw", "dw_weight"};
    return make_append(deps, integrator::dependencies());
}
std::string euler_sde::type() const{
    return "euler_sde";
}
void euler_sde::postprocess(input& dat){
    integrator::postprocess(dat);
    type_constructor<euler_sde_tmpl>::create(&actual, rh_val);
    actual->postprocess(dat);
    actual->holder = holder;
    actual->setname(this->name() + "_actual");
}

const std::type_info& euler_sde::vtype() const{
    return actual->vtype();
}

