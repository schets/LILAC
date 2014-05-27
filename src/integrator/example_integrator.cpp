#include "example_integrator.h"
#include "type_register.hpp"
#include "item_heads.hpp"
#include "example_integrator_tmpl.hpp"
//Note that this function serves as a proxy for the actual version, the template class
int example_integrator::integrate(ptr_passer u, double t0, double tf){
    return actual_int->integrate(u, t0, tf);
}

std::string example_integrator::type() const{
    //note that it does not return the template type,
    //just example_integrator so it can be instantiated
    return "example_integrator";
}

std::vector<std::string> example_integrator::dependencies(){
    //rules for dependencies:
    //A name preceded with a ! is optional
    std::string deps[] = {"rval1", "!rval2", "unsigned_var", "something"};
    //always call the dependencies of the parent class
    return make_append(deps, integrator::dependencies());
}

void example_integrator::postprocess(input& dat){
    //always do the base class processing first
    integrator::postprocess();
    //create the actual integrator with the type_constructor class
    //see type_constructor.hpp for details on this
    type_constructor<example_integrator_tmpl>::create(&actual_int, rh_val);
    //rh_val is the rhs pointer that is retrieved by the base class

    //construct the actual_int class
    actual_int->holder = holder;
    actual_int->postprocess(dat);
}

//simply return actual_int
const std::type_info& vtype() const{
    return actual_int->vtype();
}
