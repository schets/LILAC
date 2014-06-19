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
#pragma once
#ifndef EULER_SDE_TMPL_HPP
#define EULER_SDE_TMPL_HPP
#include "utils/defs.hpp"
#include "utils/noise.hpp"
#include "rhs/rhs_sde.h"
#include "euler_sde.h"
#include <limits>
#include <stdlib.h>
#include "types/float_traits.hpp"
template<class T>
class euler_sde_tmpl final_def : public euler_sde{
    typedef typename float_traits<T>::type real_type;
    real_type stepsize;
    T* restr f0, * restr w0, *restr bfnc;
    rhs_sde* func;
    int calc_dw;
    real_type dw_weight;
    public:
    const std::type_info& vtype() const;
    void postprocess(input& dat);
    std::string type() const;
    int integrate(ptr_passer u, double t0, double tf);

};
template<class T>
int euler_sde_tmpl<T>::integrate(ptr_passer u0, double ts, double tf){
    T* restr vals = u0;
    size_t steps =  std::ceil((tf-ts)/stepsize);
    double dt = (tf-ts)/steps;
    double tc = ts;
    //separate real_type for use as a multiplying noise factor
    real_type dt_mul = dt;
    ALIGNED(vals);
    ALIGNED(f0);
    ALIGNED(w0);
    ALIGNED(bfnc);
    for(size_t i = 0; i < steps; i++){
        if(std::abs(dw_weight) > std::numeric_limits<real_type>::epsilon())
        {
            gaussian_noise(w0, dimension, dt*dw_weight);
            if(func){
                func->mod_w(w0, tc);
            }
            //only calculate coefficients for dw if needed
            if(calc_dw){
                func->dxdt(vals, f0, tc);
                func->dwdt(vals, bfnc, tc);
                for(size_t j = 0; j < dimension; j++){
                    vals[j] = vals[j] + dt_mul*f0[j] + bfnc[j]*w0[j];
                }
            }
            else{
                rh_val->dxdt(vals, f0, tc);
                for(size_t j = 0; j < dimension; j++){
                    vals[j] = vals[j] + dt_mul*f0[j] + w0[j];
                }
            }
        }
        else{
            rh_val->dxdt(vals, f0, tc);
            for(size_t j = 0; j < dimension; j++){
                vals[j] = vals[j] + dt_mul*f0[j];
            }
        }

        tc += dt;
    }
    return 0;
}
template<class T>
const std::type_info& euler_sde_tmpl<T>::vtype() const{
    return typeid(T);
}
template<class T>
std::string euler_sde_tmpl<T>::type() const{
    return std::string("euler_sde_tmpl<") + this->vname() + ">";
}
template<class T>
void euler_sde_tmpl<T>::postprocess(input& dat){
    integrator::postprocess(dat);
    //since the rh_val is already obtained by the map,
    //may as well just check instead of going through retrieve
    dat.retrieve(calc_dw, "calc_dw", this);
    func = dynamic_cast<rhs_sde*>(rh_val);
    if(calc_dw){
        if(func == 0){
            err(std::string("The rhs, ") + rh_val->name() + ", is not part of the rhs_sde inheritance heirarchy." +
                    " Consider setting calc_dw to 0 to allow for a constant multiple of the noise factor",
                    this->type() + "::postprocess", "integrator/euler_sde_tmpl.hpp", FATAL_ERROR);
        }
    }
    if(!rh_val->compare<T>()){
        err("Bad rhs type passed to euler_sde integrator", "euler_sde_tmpl::postprocess",
                "integrator/euler_sde_tmpl.h", FATAL_ERROR);
    }
    //This is a temporay value so that the retrieve is always double
    double _dw_weight;
    dat.retrieve(_dw_weight, "dw_weight", this);
    dw_weight=_dw_weight;
    double _stepsize;
    dat.retrieve(_stepsize, "stepsize", this);
    stepsize=_stepsize;
    if(stepsize <= 0){
        err("stepsize is invalid, must be >= 0", "euler_sde_tmpl::postprocess",
                "integrator/euler_sde_tmpl.hpp", dat["stepsize"], FATAL_ERROR);
    }
    memp.create(dimension, &f0, &w0, &bfnc);
}
#endif
