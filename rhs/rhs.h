#ifndef RHS_H
#define RHS_H
#include "defs.h"
/*
 * This file has the definitions for the RHS class in it. All classed that inherit rhs
 * must provide a calling function that takes in the function values at a certain point
 * and an array that will contain the derivatives at that time, along with the current time
 * The return value of this function can be an error code, although I have not implemented
 * any error handling yet. The C++ try/catch functionality wreaks havoc on optimizations
 * so I don't think it would be a good idea to use it in any of the routines here
 *
 *
 * The RHS class is effectively the definition of the equations that are being solved.
 * From the perspective of the integrator, and the solver, this is a black box that
 * creates a function's derivative given the value. They just see an rhs* and call dxdt.
 * */


//input pointers are not constant as they are used in the FFT routines
class rhs{
    //This is the base class for the RHS functionality
    protected:
        size_t dimension;
    public:
        inline size_t dim() const{
            return dimension;
        }
        rhs(size_t dimen): dimension(dimen){}
        virtual int dxdt(comp* restr x, comp* restr dx, double t) = 0;
        virtual ~rhs(){}
};
class rhs_x:public rhs{
    //test class for x' = x
    public:
        rhs_x(size_t dimen): rhs(dimen){}
        virtual int dxdt(comp* restr x, comp* restr dx, double t);
};
class rhs_const:public rhs{
    //test class for x'=const
    const comp const_val;
    public:
    rhs_const(comp cval, size_t dimen):const_val(cval), rhs(dimen){
    }
    virtual int dxdt(comp* restr x, comp* restr dx, double t);
};
class rhs_CNLS: public rhs{
    //This class defines the rhs equations for the coupled CNLS
    comp * restr u1, * restr u2, * restr uf1, *restr uf2, * restr comp_in,
         * restr comp_out, * restr comp_out_r, * restr comp_in_r;
    double * restr sq1, * restr sq2, * restr k, * restr ksq;
    fftw_plan ffor, fback;
    const double g0, e0, dt, LENGTH_T;
    const size_t NUM_TIME_STEPS;
    public:
        rhs_CNLS(size_t dimen, double g, double e, double dt, double tlen, size_t n_steps);
        ~rhs_CNLS();
        virtual int dxdt(comp* restr x, comp* restr dx, double t);
};
#endif
