#ifndef EULER_SDE_H
#define EULER_SDE_H
#include "integrator.h"
//!Dormand-Prince integrator
/*!
 * This class defines a Dormand-Prince integrator, the same type used by the
 * Matlab function ODE45
 */
class euler_sde:public integrator{
    protected:
    std::shared_ptr<euler_sde> actual;
    public:
    const std::type_info& vtype() const;
    void postprocess(input& dat);
    std::vector<std::string> dependencies() const;
    std::string type() const;
    int integrate(void* restr u, double t0, double tf);
    ~euler_sde();
};


#endif
