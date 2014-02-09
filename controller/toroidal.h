#ifndef TOROIDAL_H
#define TOROIDAL_H
#include "controller.h"
//!This class defines the toroidal search controller
/*!
 * This class defines a controller that can be used for searching a toroidal parameter space
 * It increments a set of variables at specified increments a set number of times. In the future,
 * this class will also be able to perform searches until the parameter space has been
 * successfully examined
 */
class toroidal:public controller{
    int iterations;
    double initial_inc;
    double mul_fac;
    public:
    virtual void addvar(variable* v); 
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(std::map<std::string, item*>& inval);
        virtual std::string type() const;
        virtual void control(comp* u, objective* obj);
};
#endif
