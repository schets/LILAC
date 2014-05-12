#ifndef RHS_H
#define RHS_H
#include "engine/item_dim.h"
#include "../utils/vartype.hpp"
#include "ptr_passer.hpp"
//!The rhs base class
/*! \class rhs
 * The RHS class provides the RHS of an equation du/dt = f(u)
 * This is an abstract virtual class that provides a uniform interface to the
 * integrator class. Aside from the standard item functions, derived classes must implement
 * a function dxdt which loads an array with the derivative at a certain time given the time and
 * function value
 *
 * The array is a void pointer as to allow users to 
 * \sa integrator  
 * 
 * */
class rhs:public item_dim, public vartype{

        public:
        static rhs* create(std::string tname);
        std::vector<std::string> dependencies() const;
        void postprocess(input& dat);
         
        /*!\brief
         * Creates the base rhs class with a dimension=dimen
         */
        rhs(){};

        /*!
         * Initializes the generic initial condition for the problem at hand
         */
        virtual void initial_condition(ptr_passer x, size_t len)=0;

        //!Wrapper for intial_condition that assumes the length is equal to dimension
        inline void initial_condition(ptr_passer x){
            initial_condition(x, dimension);
        }
        /*!\brief
         * Stores the derivative in dx given the time t and current x
         */
        virtual int dxdt(ptr_passer x, ptr_passer dx, double t) = 0;
        virtual ~rhs(){};
};


#endif
