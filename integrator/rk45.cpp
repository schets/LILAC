#include "integrator.h"
#include "../utils/comp_funcs.h"
#include "../rhs/rhs.h"
#include "rk45.h"
#include "rk45_tmpl.h"

int rk45::integrate(void* restr u, double t0, double tf){
    return actual->integrate(u, t0, tf);
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
void rk45::postprocess(std::map<std::string, item*>& dat){
    integrator::postprocess(dat);
    if(rh_val->compare<comp>()){
        actual = new rk45_tmpl<comp>();
    }
    else if(rh_val->compare<double>()){
        actual = new rk45_tmpl<double>();
    }
    else if(rh_val->compare<float>()){
        actual = new rk45_tmpl<float>();
    }
    else{
        std::string errmess = "rhs type ";
        errmess.append(rh_val->vname());
        errmess.append(" not recognized");
        err(errmess, "rk45::postprocess", "integrator/rk45.cpp", FATAL_ERROR);
    }
    actual->postprocess(dat);
}
void rk45::parse(std::string inval){
    inval[0]=0;//This prevents unused parameter warnings
};
void rk45::print() const{
};
