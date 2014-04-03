#ifndef stable_h
#define stable_h
#include "simulation.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
#include "utils/vartype.hpp"
//!A system that attempts to reach an equilibrium
/*! This class defines a system that tries to reach an equilibrium
 * The actual implementation lets the user define a function get_change, which
 * gets the change between the two most recent iterations, and a function
 * iterate_system, which iterates the system forwards one time step
 */

const static int num_min = 10;
class stable:public simulation, public vartype{
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
        int round;
        int bad_res;
    public:
        double simulate();
        virtual ~stable(){};
};
//!A class representing an ODE system that is integrated towards stability
/*!
 * This class represents an ODE system that undergoes integration.
 * This class doesn't add too much, but since simple ode systems are so ubiquitous,
 * I think having it encapsulated in a class is important so one doesn't have to
 * rewrite a system every time they want to integrate an ode.
 *
 * Note that this class is really a wrapper to a statically typed template class.
 * This allows the system to choose the type at runtime, and generally be ignorant of the
 *  variable type of the defined problem. 
 */
class stable_ode:public stable{
    private:
        stable_ode* actual;
    protected:
        double tcur;
        comp* restr ucur, * restr ulast;
        //!This is the length of integration for each step
        double int_len;
        //!This is the starting time
        double t0; 
        //!This defines the integrator being used
        integrator* inter;
        //T!his applies operations before the integration occurs
        virtual void pre_integration_operations();
        //!This applies operations ofter the integration occurs
        virtual void post_integration_operations();
        virtual void iterate_system();
    public:
        const std::type_info& vtype() const;
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
    private:
        virtual void pre_integration_operations();
        virtual void post_integration_operations();
    protected:
        double* t;
        double* help;
        size_t nts;
        size_t num_pulses;
        fftw_plan ffor, fback;
        //!Applies operations prior to the fft and integration
        virtual void pre_fft_operations();
        //!Applies operations after the fft but before the integrations
        virtual void post_fft_operations();
        //!Applies operations after the integration but prior to the ifft
        virtual void pre_ifft_operations();
        //!Applies operations after the integration and ifft()
        virtual void post_ifft_operations();
    public:
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(std::map<std::string, item*>& invals);
        virtual std::string type() const;
        virtual ~stable_spectral_pde_1d();
};
#endif
