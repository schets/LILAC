#pragma once
#ifndef EULER_SDE_TMPL_HPP
#define EULER_SDE_TMPL_HPP
#include "utils/defs.hpp"
#include "utils/noise.h"
#include "rhs/rhs_sde.h"
#include <stdlib.h>
template<class T>
class euler_sde_tmpl{
    double dt;
    size_t dimension;
    void dxdt(T* invals, T* outvals, double t);
    void dxdw(T* invals, T* outvals, double t);
    T* restr f0, * restr w0, *restr bfnc;
    rhs_sde* func;
    public:
        int integrate(T* vals, double ts, double tf){
            size_t steps = (size_t) (tf-ts)/dt;
            double tc = ts;
            for(size_t i = 0; i < steps; i++){
                gaussian_noise_double(w0, dimension, dt);
                func->dxdt(vals, f0, tc);
                func->dwdt(vals, bfnc, tc);
                for(size_t j = 0; j < dimension; j++){
                    vals[j] += dt*f0[j] + bfnc[j]*w0[j];
                }
                tc += dt;
            }
            if(tc == tf){
                return 0;
            }
            //perform final step to get to tf
            dt = tf-tc;
            gaussian_noise_double(w0, dimension, dt);
            func->dxdt(vals, f0, tc);
            func->dwdt(vals, bfnc, tc);
            for(size_t j = 0; j < dimension; j++){
                vals[j] += dt*f0[j] + bfnc[j]*w0[j];
            }
            return 0;
        }
};
#endif

