#ifndef RHS_H
#define RHS_H
#include "defs.h"
//can't use arbitrary types here since gsl requires the use of 
int rhs(double t, double* curx, double* dx, void* params);
typedef struct odeparams{
    comp *u1, *u2, *uf1, *uf2;
    fftw_plan p1, p2, pb1, pb2;
    real alpha1, alpha2, alpha3, alpha4;
} odeparams;
#endif
