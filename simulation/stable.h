#ifndef stable_h
#define stable_h
#include "simulation.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
//!A system that attempts to reach an equilibrium
/*! This class defines a system that tries to reach an equilibrium
 * The actual implementation lets the user define a function get_change, which
 * gets the change between the two most recent iterations, and a function
 * iterate_system, which iterates the system forwards one time step
 */
class stable:public simulation{
    protected:
        //!Maximum number of times the system is iterated before assuming an unstable state
        int max_iterations;
        //!The threshold of change between two iterations for which the system will be considered stable
        double change_threshold;
        //!Returns the difference between the two most recent iterations
        virtual double get_change() = 0;
        //iterates the system forwards in time
        virtual void iterate_system() = 0;
        virtual std::vector<std::string> dependencies() const = 0;
        virtual void postprocess(std::map<std::string, item*>& invals) = 0;
        int num_gone;
    public:
        double simulate();
        virtual ~stable(){};
};
//!A class representing an ODE system that is integrated towards stability
/*!
 * This class represents an ODE system that undergoes integration.
 * This class doesn't add too much, but since simple ode systems are so ubiquitous,
 * I think having it encapsulated in a class is important so one doesn't have to
 * rewrite a system every time they want to integrate an ode
 */
class stable_ode:public stable{
    protected:
        double tcur;
        comp* restr ucur, * restr ulast;
        //!This is the length of integration for each step
        double int_len;
        //!This is the starting time
        double t0; 
        //!This defines the rhs that is being integrated
        rhs* rh;
        //!This defines the integrator being used
        integrator* inter;
        //T!his applies operations before the integration occurs
        virtual void pre_integration_operations();
        //!This applies operations ofter the integration occurs
        virtual void post_integration_operations();
        virtual void iterate_system();
    public:
        double score();
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(std::map<std::string, item*>& invals);
        virtual std::string type() const;
        virtual ~stable_ode();
};

//!This class represents the spectral integration of a PDE
/*!
 * This class will represent the integration of a 1d spectral ODE.
 * I'm not sure how to properly generalize it to multiple variables,
 * so I'll just leave it blank for now
 */
class stable_spectral_pde_1d:public stable_ode{
};
#endif
