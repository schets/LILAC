#include "ode.h"
#include "ode_tmpl.hpp"
#include "utils/type_constructor.hpp"
std::string ode::type() const{
    return actual->type();
}
const std::type_info& ode::vtype() const{
    return actual->vtype();
}
std::vector<std::string> ode::dependencies()const{
    std::string deps[] = {"!t0", "!tw", "!w_step", "tf", "integrator"};
    return make_append(deps, simulation::dependencies());
}
void ode::postprocess(input& in){
    simulation::postprocess(in);
    //find way to redirect this to actual later on?
    retrieve(inter, in["integrator"], this);
    type_constructor<ode_tmpl>::create(&actual, inter);
    actual->setname(this->name() + "_actual");
    actual->holder = holder;
    actual->postprocess(in);
    add_as_parent(actual);
}
double ode::simulate(){
    return actual->simulate();
}
