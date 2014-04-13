#ifndef RK4_H
#define RK4_H
#include "integrator.h"
//!Dormand-Prince integrator
/*!
 * This class defines a Dormand-Prince integrator, the same type used by the
 * Matlab function ODE45
 */
class rk4:public integrator{
    protected:
    std::shared_ptr<rk4> actual;
    public:
    const std::type_info& vtype() const;
    void postprocess(input& dat);
    std::vector<std::string> dependencies() const;
    std::string type() const;
    int integrate(void* restr u, double t0, double tf);
    ~rk4();
};


#endif
