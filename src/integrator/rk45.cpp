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
#include "rk45.h"
#include "rk45_tmpl.hpp"
#include "utils/item_heads.hpp"
#include "../utils/comp_funcs.hpp"
#include "types/type_constructor.hpp"
#include "types/type_register.hpp"

template class type_register<rk45>;

int rk45::integrate(ptr_passer u, double t0, double tf){
    return actual->integrate(u, t0, tf);
}
const std::type_info& rk45::vtype() const{
    return actual->vtype();
}
rk45::~rk45(){
}
std::vector<std::string> rk45::dependencies() const{
    std::string deps[] = {"dt_init", "dt_min", "dt_max", "relerr", "abserr"};
    return make_append(deps, integrator::dependencies());
}
std::string rk45::type() const{
    return "rk45";
}
void rk45::postprocess(input& dat){
    integrator::postprocess(dat);
    type_constructor<rk45_tmpl>::create(&actual, rh_val);
    actual->setname(this->name() + "_actual");
    actual->holder = holder;
    actual->postprocess(dat);
    this->add_as_parent(actual);
}
