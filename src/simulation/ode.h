#pragma once
#ifndef ODE_H
#define ODE_H
#include "simulation.h"
#include "integrator/integrator.h"
class ode: public simulation, public vartype{
    std::shared_ptr<ode> actual;
    protected:
    integrator* inter;
    public:
    std::string type() const;
    const std::type_info& vtype() const;
    std::vector<std::string> dependencies() const;
    void postprocess(input& in);
    double simulate();
};
#endif
