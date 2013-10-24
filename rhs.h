#ifndef RHS_H
#define RHS_H
#include "defs.h"
//can't use arbitrary types here since gsl requires the use of 
int rhs(double t, double* restr curx, double* restr dx, void* params);
typedef struct odeparams{
    comp * restr u1, * restr u2, * restr uf1, *restr uf2, *restr comp_in,
         * restr comp_out, * restr comp_out_r;
    real * restr comp_in_r, * restr sq1, * restr sq2, * restr k, * restr ksq;
    fftw_plan p1, p2, pb1, pb2, c_r, c_c;
    real alpha1, alpha2, alpha3, alpha4;
    real g0, e0, dt;
} odeparams;
#endif
