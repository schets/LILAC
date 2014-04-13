#include "rhs_sde.h"
std::vector<std::string> rhs_sde::dependencies() const{
    return rhs::dependencies();
}

void rhs_sde::postprocess(input& dat){
    rhs::postprocess(dat);
}

