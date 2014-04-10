#pragma once
#ifndef EULER_SDE_TMPL_HPP
#define EULER_SDE_TMPL_HPP
#include "utils/defs.hpp"
#include "utils/noise.h"
template<class T>
class euler_sde_tmpl{
    double dt;
    size_t dimension;
    void dxdt(T* invals, T* outvals, double t);
    void dxdw(T* invals, T* outvals, double t);
    T* restr f0, * restr w0;
    public:
        int integrate(T* vals, double ts, double tf){
            size_t steps = (size_t) (tf-ts)/dt;
            double tc = ts;
            for(size_t i = 0; i < steps; i++){
                gaussian_noise_double(w0, dimension, dt)
            }
            return 0;
        }
};
#endif

