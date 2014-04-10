#include "../utils/comp_funcs.h"
#include "rk4.h"
#include "rk4_tmpl.hpp"
#include "utils/type_constructor.hpp"
int rk4::integrate(void* restr u, double t0, double tf){
    return actual->integrate(u, t0, tf);
}
rk4::~rk4(){
}
std::vector<std::string> rk4::dependencies() const{
    std::string deps[] = {"step_size"};
    return appendvec(std::vector<std::string>(deps, deps+1), integrator::dependencies());
}
std::string rk4::type() const{
    return "rk4";
}
void rk4::postprocess(std::map<std::string, std::shared_ptr<item> >& dat){
    integrator::postprocess(dat);
    type_constructor<rk4_tmpl>::create(&actual, rh_val);
    actual->postprocess(dat);
}

const std::type_info& rk4::vtype() const{
    return actual->vtype();
}
