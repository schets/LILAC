#include "integrator.h"
#include "int_imp.h"
#include "comp_funcs.h"
std::vector<std::string> integrator::dependencies() const {
    return item_dim::dependencies();
}
void integrator::postprocess(std::map<std::string, item*>& dat){
    item_dim::postprocess(dat); 
}
item* integrator::create(std::string inval){
    if(inval=="rk4"){
        return new rk4();
    }
    inval.append(" is not a valid integrator name");
    err(inval, "integrator::create", "integrator/integrator.cpp", FATAL_ERROR);
    return 0;
}
