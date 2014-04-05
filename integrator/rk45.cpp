#include "integrator.h"
#include "../utils/comp_funcs.h"
#include "../rhs/rhs.h"
#include "rk45.h"
#include "rk45_tmpl.hpp"
#include "utils/type_constructor.hpp"
int rk45::integrate(void* restr u, double t0, double tf){
    return actual->integrate(u, t0, tf);
}
const std::type_info& rk45::vtype() const{
    return actual->vtype();
}
rk45::~rk45(){
    delete actual;
}
std::vector<std::string> rk45::dependencies() const{
    std::string deps[] = {"dt_init", "dt_min", "dt_max", "relerr", "abserr"};
    return appendvec(std::vector<std::string>(deps, deps+5), integrator::dependencies());
}
std::string rk45::type() const{
    return "rk45";
}
void rk45::postprocess(std::map<std::string, std::shared_ptr<item> >& dat){
    integrator::postprocess(dat);
    type_constructor<rk45_tmpl>::create(&actual, rh_val);
    actual->postprocess(dat);
}
void rk45::parse(std::string inval){
    inval[0]=0;//This prevents unused parameter warnings
};
void rk45::print() const{
};
