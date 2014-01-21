#ifndef SOLVER_HPP
#define SOLVER_HPP
#include "defs.h"
#include "rhs/rhs.h"
#include "parser/item.h"
//this class defines the interface to the ODE integrators. It treats the 
//rhs of the equation as a black-box rhs* type, and integrates from t0 to tf
//with u being the initial condition. The return of integrate is an error code,
//and the end result is stored in the input pointer.
//The internal memory stuffs of each class are dealt with by the constructors
//and destructors and are effectively opaque to the rest of the program
class integrator:public item_dim{
    public:
        static item* create(std::string inval);
        virtual void postprocess(std::map<std::string, item*>& inval);
        virtual void parse(std::string inval) = 0;
        virtual std::vector<std::string> dependencies() const = 0;
        virtual std::string type() const = 0;
        virtual int integrate(rhs* rh, comp* restr u, double t0, double tf) = 0;
        virtual ~integrator(){}
};
//fixed step runge kutta integrator


#endif
