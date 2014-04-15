#include "rk4.h"
#include "rk4_tmpl.hpp"
#include "utils/type_constructor.hpp"
#include "utils/comp_funcs.hpp"
int rk4::integrate(void* restr u, double t0, double tf){
    return actual->integrate(u, t0, tf);
}
rk4::~rk4(){
}
std::vector<std::string> rk4::dependencies() const{
    std::string deps[] = {"stepsize"};
    return make_append(deps, integrator::dependencies());
}
std::string rk4::type() const{
    return "rk4";
}
void rk4::postprocess(input& dat){
    integrator::postprocess(dat);
    type_constructor<rk4_tmpl>::create(&actual, rh_val);
    actual->postprocess(dat);
    this->add_as_parent(actual);
    actual->setname(this->name() + "_actual");
}

const std::type_info& rk4::vtype() const{
    return actual->vtype();
}
