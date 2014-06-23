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
#include "rk4.h"
#include "item_heads.hpp"
#include "types/float_traits.hpp"
template<class T>
class rk4_tmpl:public rk4 {
    typedef typename float_traits<T>::type real_type;
    protected:
    T* restr f0, * restr f1, * restr f2, * restr f3;
    T* restr u_calc;
    rhs* func;
    real_type stepsize;
    public:
    virtual const std::type_info& vtype() const;
    void postprocess(input& dat);
    std::string type() const;
    int integrate(ptr_passer u, double t0, double tf);
    virtual ~rk4_tmpl();
};
template<class T>
const std::type_info& rk4_tmpl<T>::vtype()const{
    return typeid(T);
}
template<class T>
rk4_tmpl<T>::~rk4_tmpl(){
 }

template<class T>
int rk4_tmpl<T>::integrate(ptr_passer _u0, double t0, double tf)
    //    Using code from John Burkardt as a guideline
{
    T* restr u0 = _u0;
    ALIGNED(u0);
    ALIGNED(f0);
    ALIGNED(f1);
    ALIGNED(f2);
    ALIGNED(f3);
    ALIGNED(u0);
    ALIGNED(u_calc);
    const size_t m = dimension;
    const double t_diff = tf-t0;
    const size_t steps = ceil(t_diff/stepsize);
    const real_type dt = t_diff/steps;
    //do steps integrations
    //each loop iteration is a runge_kutta timestep
    for(size_t j = 0; j < steps; j++){
        size_t i;
        real_type t1=t0+dt/2.0;
        real_type t2=t0+dt;
        //
        //  Get four sample values of the derivative.
        //
        func->dxdt(u0, f0, t0);

        for ( i = 0; i < m; i++ ){
            u_calc[i] = u0[i] + dt * f0[i] / 2.0;
        }

        func->dxdt(u_calc, f1, t1);
        for ( i = 0; i < m; i++ ){
            u_calc[i] = u0[i] + dt * f1[i] / 2.0;
        }

        func->dxdt(u_calc, f2, t1); 
        for ( i = 0; i < m; i++ ){
            u_calc[i] = u0[i] + dt * f2[i];

        }

        func->dxdt(u_calc, f3, t2); 
        for ( i = 0; i < m; i++ ){
            u0[i] = u0[i] + (dt * ( f0[i]  + 2.0 * f1[i] + 2.0 * f2[i] + f3[i] ) / 6.0);
        }

        t0 += dt;
    }
    return 0;
}
template<class T>
std::string rk4_tmpl<T>::type()const{
    return std::string("rk4_tmpl<") + this->vname() + ">";
}
template<class T>
void rk4_tmpl<T>::postprocess(input& dat){
    integrator::postprocess(dat);
    if(!rh_val->compare<T>()){
        err("Bad rhs type passed to rk4 integrator", "rk4_tmpl::postprocess",
                "integrator/rk4_tmpl.h", FATAL_ERROR);
    }
    this->add_as_parent(rh_val);
    dat.retrieve(stepsize, "stepsize", this);
    if(stepsize <= 0){
        err("stepsize is invalid, must be >= 0", "rk4_tmpl::postprocess",
                "integrator/rk4.hpp", dat["stepsize"], FATAL_ERROR);
    }
    memp.create(dimension, &f0, &f1, &f2, &f3, &u_calc);
}

