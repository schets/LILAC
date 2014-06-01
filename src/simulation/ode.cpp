#include "ode.h"
#include "ode_tmpl.hpp"
#include "types/type_constructor.hpp"
#include "types/type_register.hpp"
template class type_register<ode>;
/*!
 * This function returns the type name of the ode class, "ode"
 */
std::string ode::type() const{
    return "ode";
}
/*!
 * Returns the variable type being used by the object,
 */
const std::type_info& ode::vtype() const{
    return actual->vtype();
}

/*!
 * Returns the dependencies of the ode class.
 * The ode class depends on:
 *      - double tf: Ending time of the integration
 *
 *      - integrator: Type of interator being used, must be a type that inherits from integrator class
 *
 * And has optional parameters: 
 *      - double t0: Starting time of integration, default value of 0
 *
 *      - double tw: Starting time of writing, default value of t0
 *
 *      - double w_step: Interval to record data, default value of -1.
 *
 * If w_step<=0 (to machine precision) or tw>= tf, then no recording will be done
 * Only the final value will be saved
 */
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
