#include "../utils/comp_funcs.hpp"
#include "euler_sde.h"
#include "euler_sde_tmpl.hpp"
#include "utils/type_constructor.hpp"
#include "utils/type_register.hpp"
template class type_register<euler_sde>;

int euler_sde::integrate(ptr_passer u, double t0, double tf){
    return actual->integrate(u, t0, tf);
}
euler_sde::~euler_sde(){
}
std::vector<std::string> euler_sde::dependencies() const{
    std::string deps[] = {"stepsize", "calc_dw", "dw_weight"};
    return make_append(deps, integrator::dependencies());
}
std::string euler_sde::type() const{
    return "euler_sde";
}
void euler_sde::postprocess(input& dat){
    integrator::postprocess(dat);
    type_constructor<euler_sde_tmpl>::create(&actual, rh_val);
    actual->postprocess(dat);
    actual->holder = holder;
    actual->setname(this->name() + "_actual");
}

const std::type_info& euler_sde::vtype() const{
    return actual->vtype();
}
