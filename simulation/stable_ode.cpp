#include "stable.h"
#include "stable_ode_tmpl.hpp"
#include "utils/type_constructor.hpp"
#include "utils/comp_funcs.hpp"
//stable_ode functions

/*!
 * This is the function that iterates the ODE system forwards.
 * It applies the operator pre_integration_operations, then integrates
 * from tcur to tcur+tlen, and then applies the operator post_integration_operations.
 * \sa stable::simulate
 */
void stable_ode::iterate_system(){
    actual->iterate_system();
}

void stable_ode::postprocess(std::map<std::string, item*>& invals){
    stable::postprocess(invals);
    invals["integrator"]->retrieve(&inter, this);
    type_constructor<stable_ode_tmpl>::create(&actual, inter);
    actual->postprocess(invals);
}

stable_ode::~stable_ode(){
    delete actual;
}
void stable_ode::pre_integration_operations(){
    actual->pre_integration_operations();
}
void stable_ode::post_integration_operations(){

    actual->post_integration_operations();
}
std::string stable_ode::type() const{
    return actual->type();
}
std::vector<std::string> stable_ode::dependencies() const{
    std::string deps[] = {"integrator", "t0", "int_len"};
    return appendvec(std::vector<std::string>(deps, deps+3), stable::dependencies());
}
double stable_ode::score(){
    return actual->score();
}
const std::type_info& stable_ode::vtype() const{
    return actual->vtype();
}
double stable_ode::get_change(){
    return actual->get_change();
}
