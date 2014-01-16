#ifndef SOLVER_HPP
#define SOLVER_HPP
#include "defs.h"
#include "rhs/rhs.h"
//this class defines the interface to the ODE integrators. It treats the 
//rhs of the equation as a black-box rhs* type, and integrates from t0 to tf
//with u being the initial condition. The return of integrate is an error code,
//and the end result is stored in the input pointer.
//The internal memory stuffs of each class are dealt with by the constructors
//and destructors and are effectively opaque to the rest of the program
class integrator{
    protected:
        const size_t dimension;
    public:
        integrator(size_t dimen): dimension(dimen){};
        integrator(): dimension(0){}
        inline size_t dim(){return dimension;}
        virtual int integrate(rhs* rh, comp* restr u, double t0, double tf) = 0;
        virtual ~integrator(){}
};
//fixed step runge kutta integrator


#endif
