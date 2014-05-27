#pragma once
#ifndef EULER_SDE_TMPL_HPP
#define EULER_SDE_TMPL_HPP
#include "utils/defs.hpp"
#include "utils/noise.h"
#include "rhs/rhs_sde.h"
#include "euler_sde.h"
#include <limits>
#include <stdlib.h>
template<class T>
class euler_sde_tmpl: public euler_sde{
    double stepsize;
    T* restr f0, * restr w0, *restr bfnc;
    rhs_sde* func;
    int calc_dw;
    double dw_weight;
    public:
    const std::type_info& vtype() const;
    void postprocess(input& dat);
    std::string type() const;
    int integrate(ptr_passer u, double t0, double tf);

};
template<class T>
int euler_sde_tmpl<T>::integrate(ptr_passer u0, double ts, double tf){
    T* restr vals = u0.get<T>();
    size_t steps =  ceil((tf-ts)/stepsize);
    double dt = (tf-ts)/steps;
    double tc = ts;
    ALIGNED(vals);
    ALIGNED(f0);
    ALIGNED(w0);
    ALIGNED(bfnc);
    for(size_t i = 0; i < steps; i++){
        if(std::abs(dw_weight) > std::numeric_limits<double>::epsilon())
        {
            gaussian_noise(w0, dimension, dt*dw_weight);
            //only calculate coefficients for dw if needed
            if(calc_dw){
                func->dxdt(vals, f0, tc);
                func->dwdt(vals, bfnc, tc);
                for(size_t j = 0; j < dimension; j++){
                    vals[j] = vals[j] + dt*f0[j] + bfnc[j]*w0[j];
                }
            }
            else{
                rh_val->dxdt(vals, f0, tc);
                for(size_t j = 0; j < dimension; j++){
                    vals[j] = vals[j] + dt*f0[j] + w0[j];
                }
            }
        }
        else{
            rh_val->dxdt(vals, f0, tc);
            for(size_t j = 0; j < dimension; j++){
                vals[j] = vals[j] + dt*f0[j];
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
    retrieve(calc_dw, dat["calc_dw"], this);
    if(calc_dw){
        func = dynamic_cast<rhs_sde*>(rh_val);
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
    retrieve(dw_weight, dat["dw_weight"], this);
    retrieve(stepsize, dat["stepsize"], this);
    if(stepsize <= 0){
        err("stepsize is invalid, must be >= 0", "euler_sde_tmpl::postprocess",
                "integrator/euler_sde_tmpl.hpp", dat["stepsize"], FATAL_ERROR);
    }
    memp.create(dimension, &f0, &w0, &bfnc);
}
#endif
