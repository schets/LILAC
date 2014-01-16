#ifndef INT_IMP_H
#define INT_IMP_H
#include "integrator.h"
class rk4:public integrator{
    const size_t steps;
    comp* restr f0, * restr f2, * restr f3, * restr f1;
    comp* restr u_calc;
    public:
    virtual int integrate(rhs* rh, comp* u, double t0, double tf);
    rk4(size_t step, size_t dimen);
    ~rk4();
};
#endif
