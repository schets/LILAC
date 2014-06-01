#include "integrator.h"
#include "rk45.h"
#include "rk45_tmpl.hpp"
#include "utils/item_heads.hpp"
#include "../utils/comp_funcs.hpp"
#include "types/type_constructor.hpp"
#include "types/type_register.hpp"

template class type_register<rk45>;

int rk45::integrate(ptr_passer u, double t0, double tf){
    return actual->integrate(u, t0, tf);
}
const std::type_info& rk45::vtype() const{
    return actual->vtype();
}
rk45::~rk45(){
}
std::vector<std::string> rk45::dependencies() const{
    std::string deps[] = {"dt_init", "dt_min", "dt_max", "relerr", "abserr"};
    return make_append(deps, integrator::dependencies());
}
std::string rk45::type() const{
    return "rk45";
}
void rk45::postprocess(input& dat){
    integrator::postprocess(dat);
    type_constructor<rk45_tmpl>::create(&actual, rh_val);
    actual->setname(this->name() + "_actual");
    actual->holder = holder;
    actual->postprocess(dat);
    this->add_as_parent(actual);
}
