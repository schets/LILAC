#ifndef RHS_H
#define RHS_H
#include "../utils/defs.h"
#include "../parser/item.h"
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
class rhs:public item_dim{

        public:
        static rhs* create(std::string tname);
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(std::map<std::string, item*>& dat);
        virtual std::string val_type() const = 0;
        /*!\brief
         * Creates the base rhs class with a dimension=dimen
         */
        rhs(){};
        /*!\brief
         * Stores the derivative in dx given the time t and current x
         */
        virtual int dxdt(void* restr x, void* restr dx, double t) = 0;
        virtual ~rhs(){};
};


#endif
