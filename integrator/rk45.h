#ifndef RK45_H
#define RK45_H

#include "../utils/defs.h"
#include "integrator.h"
//!Dormand-Prince integrator
/*!
 * This class defines a Dormand-Prince integrator, the same type used by the
 * Matlab function ODE45
 */
class rk45:public integrator{
    protected:
    double dt_init, dt_min, dt_max, relerr, abserr;
   /* comp* restr f0, * restr f1, * restr f2, * restr f3;
    comp* restr f4, * restr f5, * restr f6;
    comp* restr u_calc;
    double* restr u_calc2;*/
    integrator* actual;
    public:
    const std::type_info& vtype() const;
    //!Dummy print function
    void print() const;
    void postprocess(std::map<std::string, item*>& dat);
    //!Deprecated
    void parse(std::string inval);
    std::vector<std::string> dependencies() const;
    std::string type() const;
    int integrate(void* restr u, double t0, double tf);
    ~rk45();
};


#endif
