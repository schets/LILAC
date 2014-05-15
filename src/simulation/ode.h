#pragma once
#ifndef ODE_H
#define ODE_H
#include "simulation.h"
#include "integrator/integrator.h"
#include "type_register.hpp"
class ode: public simulation, public vartype, type_register<ode>{
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
