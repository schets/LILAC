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
#ifndef RK45_TMPL
#define RK45_TMPL
#include "integrator.h"
#include "utils/comp_funcs.hpp"
#include "rk45.h"
#include "types/float_traits.hpp"
//!Dormand-Prince integrator
/*!
 * This class defines a Dormand-Prince integrator, the same type used by the
 * Matlab function ODE45. This is the actual template implementation,
 * which allows for definitions using multiple variables
 */
template<class T>
class rk45_tmpl final_def :public rk45 {
    typedef typename float_traits<T>::type real_type;
    protected:
    //since these values aren't used to much in actual computation
    //it's fine to just have them be doubles and easily retrieve them
    double dt_init, dt_min, dt_max;
    double relerr, abserr;
    T* restr f0, * restr f1, * restr f2, * restr f3;
    T* restr f4, * restr f5, * restr f6;
    T* restr u_calc;
    real_type* restr u_calc2;
    public:
    virtual const std::type_info& vtype() const;
    void postprocess(input& dat);
    std::string type() const;
    int integrate(ptr_passer u, double t0, double tf);
    ~rk45_tmpl();
};
template<class T>
const std::type_info& rk45_tmpl<T>::vtype() const {
    return typeid(T);
}
template<class T>
int rk45_tmpl<T>::integrate(ptr_passer _u0, double t0, double tf){
    T* restr u0 = _u0;
    size_t num_fail = 0;
    real_type dt = dt_init;
    real_type dtave=0;
    size_t steps=0;


    const real_type magic_power = 1.0/6; //found from reference file, reference p.91 Ascher and Petzold
    const real_type magic_mult = .8;//magic multiplying safety factor for determining the next timestep;
    //    dorman prince integrator parameters
    MAKE_ALIGNED const static real_type a[] = {0.2, 0.3, 0.8, 8.0/9, 1, 1};
    MAKE_ALIGNED const static real_type b1 = 0.2;
    MAKE_ALIGNED const static real_type b2[] = {3.0/40, 9.0/40};
    MAKE_ALIGNED const static real_type b3[] = {44.0/45, -56.0/15, 32.0/9};
    MAKE_ALIGNED const static real_type b4[] = {19372.0/6561, -25360.0/2187, 64448.0/6561, -212.0/729};
    MAKE_ALIGNED const static real_type b5[] = {9017.0/3168, -355.0/33, 46732.0/5247, 49.0/176, -5103.0/18656};
    MAKE_ALIGNED const static real_type b6[] = {35.0/384, 0, 500.0/1113, 125.0/192, -2187.0/6784, 11.0/84};
    MAKE_ALIGNED const static real_type c4[] = {5179.0/57600, 0, 7571.0/16695, 393.0/640,
        -92097.0/339200, 187.0/2100, 1.0/40};
    MAKE_ALIGNED       const static real_type c5[] = {35.0/384, 0, 500.0/1113, 125.0/192, -2187.0/6784, 11.0/84, 0};

    real_type taui;
    taui = 0;
    real_type tcur=t0;
    real_type tauv = 0;
    for(size_t i = 0; i < dimension; i++){
        real_type taum = abs(u0[i]);
        if(taum > tau){
            taui = taum;
        }
    }
    ALIGNED(u0);
    ALIGNED(u_calc);
    ALIGNED(u_calc2);
    ALIGNED(f0);
    ALIGNED(f1);
    ALIGNED(f2);
    ALIGNED(f3);
    ALIGNED(f4);
    ALIGNED(f5);
    ALIGNED(f6);
    tauv = taui;
    tauv *= relerr;
    T* restr tmp, * restr swp, * restr u0hld;//this is used for freeing the memory later
    T* restr tmp6, * restr tmpc;
    tmp6 = f6;
    tmpc = u_calc;
    u0hld=u0;
    //used to avoid memory problems later on
    //allows for easy switching of pointers to avoid memory copies

    tmp=f0;
    //removes the need to check if the pointers should be swapped or not
    if((tcur + dt) > tf){
        dt = tf - tcur;
        err("t0-tf <= dt, consider decreasing the initial timestep. Otherwise mileage may vary",
                "rk45::integrate", "integrator/rh45.cpp", (item*)rh_val, WARNING);
    }
    int tries=0;

    //while this seems odd, it helps streamline the inner integrator loop
    rh_val->dxdt(u0, f6, tcur);
    while(tcur < tf){
        //this function=lol at compiler loop unrolling
        if((tcur + dt) > tf){
            break;
        }
        tries++;
        swp=f6;
        f6=f0;
        f0=swp;
        real_type ts = tcur;

        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + b1*f0[i]*dt;
        }
        tcur = ts + a[0]*dt;
        rh_val->dxdt(u_calc, f1, tcur); 

        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b2[0]*f0[i] + b2[1] * f1[i]);
        }
        tcur = ts + a[1]*dt;
        rh_val->dxdt(u_calc, f2, tcur);

        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b3[0]*f0[i] + b3[1] * f1[i] + b3[2]*f2[i]);
        }
        tcur = ts + a[2]*dt;
        rh_val->dxdt(u_calc, f3, tcur);

        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b4[0]*f0[i] + b4[1] * f1[i] + b4[2]*f2[i] + b4[3]*f3[i]);
        }
        tcur = ts + a[3]*dt;
        rh_val->dxdt(u_calc, f4, tcur);

        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b5[0]*f0[i] + b5[1] * f1[i] + b5[2]*f2[i] + 
                    b5[3]*f3[i] + b5[4]*f4[i]);
        }
        tcur = ts + a[4]*dt;
        rh_val->dxdt(u_calc, f5, tcur);   

        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b6[0]*f0[i] + b6[1] * f1[i] + b6[2]*f2[i] + 
                    b6[3]*f3[i] + b6[4]*f4[i] + b6[5]*f5[i]);
        }
        tcur = ts + a[5]*dt;
        rh_val->dxdt(u_calc, f6, tcur);   
        tcur = ts;
        //calculate the magnitude of the error, and the fourth/5th order arrays
        //The references that I found are for real values
        //so I substitute the magnitude of complex differences for the absolute real difference
        //not sure if this is the way to go, but sounds about right
        //internally, I do calculations with the squares to avoid calculations of sqrt
        //This is done with the inf norm
        real_type delta=1E-12;

        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(c5[0]*f0[i] + c5[1] * f1[i] + c5[2] * f2[i] + c5[3] * f3[i] + 
                    c5[4]*f4[i] + c5[5]*f5[i] + c5[6]*f6[i]);

            u_calc2[i] = abs(u0[i] + dt*(c4[0]*f0[i] + c4[1] * f1[i] + c4[2] * f2[i] + c4[3] * f3[i] + 
                        c4[4]*f4[i] + c4[5]*f5[i] + c4[6]*f6[i]) - u_calc[i]);
        }

        //seperate these two loops to allow vectorization of the first
        for(size_t i = 0; i < dimension; i++){
            real_type deltam = u_calc2[i];//square of absolute error
            if(deltam>delta){
                delta=deltam;
            }
        }
        //estimated optimal dt, smallest of the two options to ensure accuracy
        real_type dt_last = dt;
        dt =dt*magic_mult*std::pow(tauv/delta, magic_power);
        if(dt < dt_min){
            num_fail++;
            if(num_fail > 10){
                err("Estimated timestep is smaller than minimum timestep too many times, exiting",
                        "rk45::integrate", "integrator/rk45.cpp", (item*)rh_val, WARNING);
                f0=tmp;
                f6 = tmp6;
                u_calc=tmpc;
                return -1;
            }
            //std::cout << "dt= " << dt << ", dt_min=" << dt_min<<std::endl;
            dt = dt_min;
        }
        else{
            num_fail = 0;
        }
        //since dt_max is always a double, explcit specialization is needed
        dt = std::min<real_type>(dt, dt_max);
        if(delta>=tauv){
            swp=f6;
            f6=f0;
            f0=swp;
            continue;
        }
        tcur += dt_last;
        if((tcur + dt) > tf){
            dt = tf-tcur;
        }
        for(size_t i = 0; i < dimension; i++){
            real_type val = abs(u_calc[i]);
            if(val > taui){
                taui = val;
            }
        }
        steps++;
        dtave+=dt_last;
        tauv=taui;
        tauv *= relerr;
        tries=0;
        //error is acceptable, continue on
        swp = u_calc;
        u_calc = u0;
        u0 = swp;
        //instead of copying u_calc to u_0 swap pointers
    }
    //check if a memory copy needs to be done on u0
    //u0 currently points to the most recent update
    //if u0 does not point to where it originally did, re-copy the memory over
    if(u0 != u0hld){
        for(size_t i = 0; i < dimension; i++){
            u0hld[i] = u0[i];
        }
    }
    //u0, or at least that original address, now holds the final integration values
    f0=tmp;
    f6 = tmp6;
    u_calc=tmpc;
    return 0;
}
template<class T>
rk45_tmpl<T>::~rk45_tmpl(){
}


template<class T>
std::string rk45_tmpl<T>::type() const{
    return std::string("rk45_tmpl<") + this->vname() + ">";
}
template<class T>
void rk45_tmpl<T>::postprocess(input& dat){
    integrator::postprocess(dat);
    if(!rh_val->compare<T>()){
        err("Bad rhs type passed to rk45 integrator", "rk45_tmpl::postprocess",
                "rhs/rk45_tmpl.h", FATAL_ERROR);
    }
    this->add_as_parent(rh_val);
    dat.retrieve(dt_init, "dt_init", this);
    if(dt_init <= 0){
        err("dt_init is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["dt_init"], FATAL_ERROR);
    }
    dat.retrieve(dt_min, "dt_min", this);
    if(dt_min <= 0){
        err("dt_min is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["dt_min"], FATAL_ERROR);
    }
    dat.retrieve(dt_max, "dt_max", this);
    if(dt_max <= 0){
        err("dt_max is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["dt_max"], FATAL_ERROR);
    }
    dat.retrieve(relerr, "relerr", this);
    if(relerr <= 0){
        err("relerr is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["relerr"], FATAL_ERROR);
    }
    dat.retrieve(abserr, "abserr", this);
    if(abserr <= 0){
        err("abserr is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["abserr"], FATAL_ERROR);
    }
    memp.create(dimension, &f0, &f1, &f2, &f3, &f4, &f5, &f6, &u_calc, &u_calc2);
}

#endif
