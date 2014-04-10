#include "rhs_sde.h"
std::vector<std::string> rhs_sde::dependencies() const{
    return rhs::dependencies();
}

void rhs_sde::postprocess(std::map<std::string, std::shared_ptr<item> >& dat){
    rhs::postprocess(dat);
}

