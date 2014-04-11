#include "../utils/comp_funcs.h"
#include "euler_sde.h"
#include "euler_sde_tmpl.hpp"
#include "utils/type_constructor.hpp"
int euler_sde::integrate(void* restr u, double t0, double tf){
    return actual->integrate(u, t0, tf);
}
euler_sde::~euler_sde(){
}
std::vector<std::string> euler_sde::dependencies() const{
    std::string deps[] = {"step_size"};
    return make_append(deps, integrator::dependencies());
}
std::string euler_sde::type() const{
    return "euler_sde";
}
void euler_sde::postprocess(std::map<std::string, std::shared_ptr<item> >& dat){
    integrator::postprocess(dat);
    type_constructor<euler_sde_tmpl>::create(&actual, rh_val);
    actual->postprocess(dat);
}

const std::type_info& euler_sde::vtype() const{
    return actual->vtype();
}
