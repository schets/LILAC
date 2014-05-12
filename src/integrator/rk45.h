#ifndef RK45_H
#define RK45_H

#include "integrator.h"
//!Dormand-Prince integrator
/*!
 * This class defines a Dormand-Prince integrator, the same type used by the
 * Matlab function ODE45
 */
class rk45:public integrator{
    protected:
    std::shared_ptr<rk45> actual;
    public:
    const std::type_info& vtype() const;
    void postprocess(input& dat);
    std::vector<std::string> dependencies() const;
    std::string type() const;
    int integrate(ptr_passer u, double t0, double tf);
    ~rk45();
};


#endif
