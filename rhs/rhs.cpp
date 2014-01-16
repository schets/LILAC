#include "rhs.h"
#include "rhs_imp.h"
#include <cstring>
int rhs_const::dxdt(comp* restr x, comp* restr dx, double t){
    for(int i = 0; i < dimension; i++){
        dx[i] = const_val;
    }
    return 0;
}
std::vector<std::string> rhs_const::dependencies() const{
    std::string deps[] = {"const_fact", "dimension"};
    return std::vector<std::string>(deps, deps+2);
}
std::string rhs_const::type() const {
    return "rhs_const";
}
void rhs::parse(std::string s){
}

rhs* rhs::create(std::string tname){
    if(tname == "CNLS"){
        return new rhs_CNLS();
    }
    else if (tname == "const"){
        return new rhs_const();
    }
    return 0;
}
std::vector<std::string> rhs::dependencies()const {
    return std::vector<std::string>();
}
void rhs::retrieve(void* p) const{
}
