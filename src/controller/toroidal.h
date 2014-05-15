#ifndef TOROIDAL_H
#define TOROIDAL_H
#include "controller.h"
#include "utils/type_register.hpp"
//!This class defines the toroidal search controller
/*!
 * This class defines a controller that can be used for searching a toroidal parameter space
 * It increments a set of variables at specified increments a set number of times. In the future,
 * this class will also be able to perform searches until the parameter space has been
 * successfully examined
 */
class toroidal:public controller, type_register<toroidal>{
    size_t iterations;
    size_t num_int;
    double initial_inc;
    double mul_fac;
    public:
    virtual void addvar(std::weak_ptr<variable> v); 
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(input& inval);
        virtual std::string type() const;
        virtual void control(comp* u, objective* obj);
        virtual char is_good();
        virtual void pre_set();
};
#endif
