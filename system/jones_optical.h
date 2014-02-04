#ifndef CNLS_H
#define CNLS_H
#include "system.h"
#include "integrator/integrator.h"
#include "rhs/rhs.h"
#include "controller/controller.h"
#include "objective/objective.h"
#include "defs.h"
class jones_optical:public system{
    double* help, phold, t;
    comp* u0, u1;
    rhs* rh;
    integrator* inter;
    public:
    virtual void postprocess(std::map<std::string, item*>& invals);
    //WILL BE ACTUALLY MADE SOON
    virtual std::vector<std::string> dependencies() const {return std::vector<std::string>();};
    //WILL BE ACTUALLY MADE SOON
    virtual void checkpoint(){};
    virtual double simulate() = 0;
    virtual void dump()=0;
};
#endif
