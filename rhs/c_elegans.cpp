#include "c_elegans.h"
int c_elegans::dxdt(double* restr x, double* restr dx, double dt){
    double* restr dv = dx;
    double* restr v = x;
    double* restr ds = dv+dim_v;
    double* restr s = v+dim_v;
    Map<Matrix<double, Dynamic, 1>, Aligned > vmap(v, dim_v);
    Map<Matrix<double, Dynamic, 1>, Aligned > dvmap(dv, dim_v);
    Map<Matrix<double, Dynamic, 1>, Aligned > smap(s, dim_v);
    Map<Matrix<double, Dynamic, 1>, Aligned > dsmap(ds, dim_v);
    //calculate sig vector (MUST VECTORIZE THIS PART LATER)
    for(size_t i = 0; i < dim_v; i++){
        sig[i] = 1.0 / (1.0 + std::exp(beta*(vmean[i] - v[i])));
    }
    Iohm.array() = memG*(vmap.array() - memV);
    Ielec.noalias() = gelec*laplacian*vmap;
    Ichem.array() = gchem *
        (vmap.array()*(AEchem_trans*smap) .array()
         - (AEchem_trans*(smap.array()*Echem.array()).matrix()).array());
    dvmap = (-1/tau)*(Iohm+Ichem+Ielec);
    //apply forcing at proper positions
    for(auto s : inj_nodes){
        dv[s] += (-1/tau)*cur_inj;
    }
    return 0;
}
int c_elegans::dwdt(double* restr x, double* restr dx, double dt){
    std::fill(dx, dx+dimension, 1);
    return 0;
}
std::string c_elegans::type() const{
    return "c_elegans";
}
void c_elegans::postprocess(input& in){
    rhs_sde::postprocess(in);
}
