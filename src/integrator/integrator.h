#ifndef SOLVER_HPP
#define SOLVER_HPP
#include "../rhs/rhs.h"
#include "../engine/item.h"
#include "engine/item_dim.h"
#include "utils/vartype.hpp"
//!This class defines the integration base class
/*!
 * This class defines the base integration class, the interface that is used by
 * the engine to integrate differential equations
 */
class integrator:public item_dim, public vartype{
    protected:
        rhs* rh_val;
    public:
        static item* create(std::string inval);
        virtual void initial_condition(void* in, size_t len);
        inline void initial_condition(void* in){
            initial_condition(in, dimension);
        }
        virtual void postprocess(input& inval);
        //!Deprecated
        virtual void parse(std::string inval){};
        virtual std::vector<std::string> dependencies() const = 0;
        //!Returns the name of the integrator
        virtual std::string type() const = 0;
        //!Integrates the function rh
        /*!
         * This function integrates the equation du/dt=rh->dxdt(u) from t0 to tf
         * @param rh The function being integrated
         * @param u A pointer to the initial condition, also stores the final condition
         * @param t0 The initial time
         * @param tf The ending time
         */
        virtual int integrate(void* restr u, double t0, double tf) = 0;
        //!Blank destructor for integrator
        virtual ~integrator(){}
};

#endif
