#pragma once
#ifndef EULER_SDE_TMPL_HPP
#define EULER_SDE_TMPL_HPP
#include "utils/defs.hpp"
#include "utils/noise.h"
#include "rhs/rhs_sde.h"
#include "euler_sde.h"
#include <limits>
#include <stdlib.h>
#include "types/float_traits.hpp"
template<class T>
class euler_sde_tmpl final: public euler_sde{
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
    size_t steps =  ceil((tf-ts)/stepsize);
    real_type dt = (tf-ts)/steps;
    real_type tc = ts;
    ALIGNED(vals);
    ALIGNED(f0);
    ALIGNED(w0);
    ALIGNED(bfnc);
    for(size_t i = 0; i < steps; i++){
        if(std::abs(dw_weight) > std::numeric_limits<real_type>::epsilon())
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
    //This is a temporay value so that the retrieve is always double
    double _dw_weight;
    retrieve(_dw_weight, dat["dw_weight"], this);
    dw_weight=_dw_weight;
    double _stepsize;
    retrieve(_stepsize, dat["stepsize"], this);
    stepsize=_stepsize;
    if(stepsize <= 0){
        err("stepsize is invalid, must be >= 0", "euler_sde_tmpl::postprocess",
                "integrator/euler_sde_tmpl.hpp", dat["stepsize"], FATAL_ERROR);
    }
    memp.create(dimension, &f0, &w0, &bfnc);
}
#endif
