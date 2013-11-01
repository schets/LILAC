#ifndef SOLVER_HPP
#define SOLVER_HPP
extern "C"{
#include "defs.h"
}
#include "rhs/rhs.h"
class integrator{
    protected:
        const size_t dimension;
    public:
        integrator(size_t dimen): dimension(dimen){};
        integrator(): dimension(0){}
        inline size_t dim(){return dimension;}
        virtual int integrate(rhs* rh, comp* u, double t0, double tf) = 0;
        virtual ~integrator(){}
};

class rk4:public integrator{
    const size_t steps;
    comp* __restrict__ f0, * __restrict__ f2, * __restrict__ f3, * __restrict__ f1;
    comp* __restrict__ u_calc;
    public:
    virtual int integrate(rhs* rh, comp* u, double t0, double tf);
    rk4(size_t step, size_t dimen): integrator(dimen), steps(step),
        f0((comp*)malloc(dimen*sizeof(comp))), f1((comp*)malloc(dimen*sizeof(comp))),
        f2((comp*)malloc(dimen*sizeof(comp))), f3((comp*)malloc(dimen*sizeof(comp))),
        u_calc((comp*)malloc(dimen*sizeof(comp))){}

    ~rk4();
};

#endif
