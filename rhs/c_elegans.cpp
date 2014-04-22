#include "c_elegans.h"
#include "engine/input.h"
#include "utils/comp_funcs.hpp"
#include "utils/item_heads.hpp"
#include <vector>
#include <fstream>
#include "gaba_list.hpp"
constexpr size_t c_elegans::num_neur;
constexpr size_t c_elegans::dim_v;
constexpr size_t c_elegans::dim_s;
int c_elegans::dxdt(double* restr x, double* restr dx, double dt){
    double* restr dv = dx;
    double* restr v = x;
    double* restr ds = dv+dim_v;
    double* restr s = v+dim_v;
    Map<Array<double, dim_v, 1>, Aligned > vmap(v);
    Map<Array<double, dim_v, 1>, Aligned > dvmap(dv);
    Map<Array<double, dim_v, 1>, Aligned > smap(s);
    Map<Array<double, dim_v, 1>, Aligned > dsmap(ds);

    sig = 1.0 / (1.0 + (beta*(vmean -vmap)).exp());
    dsmap = ar*(sig * (1.0-smap)) - ad*smap;
    
   // Iohm = (memG*(vmap - memV));
   // Ielec.noalias() = gelec*laplacian*vmap;
  //  Ichem = (gchem *
  //      (vmap*(AEchem_trans*smap) 
  //       - (AEchem_trans*(smap*Echem).matrix())));
  
    dvmap = (-1/tau)*((memG*(vmap - memV))
            +(gelec*laplacian*vmap.matrix()).array()+
            (gchem *(vmap*(AEchem_trans*smap.matrix()).array()
              - (AEchem_trans*(smap*Echem).matrix()).array())));

    for(auto s : inj_nodes){
        dv[s] += (-1/tau)*cur_inj;
    }
    return 0;
}
int c_elegans::dwdt(double* restr x, double* restr dx, double dt){
    std::fill(dx, dx+num_neur*2, 1);
    return 0;
}
std::string c_elegans::type() const{
    return "c_elegans";
}
template<class sp_t>
void read_mat(std::string fname, sp_t& in_mat){
    std::ifstream in_f(fname);
    std::vector<Eigen::Triplet<double, int> > intr;
    while(!in_f.eof()){
        int row, col;
        double val;
        in_f >> row >> col >> val;
        intr.push_back(Triplet<double, int>(row-1, col-1, val));
    }
    in_mat.setFromTriplets(intr.begin(), intr.end());
}
void c_elegans::postprocess(input& in){
    rhs_sde::postprocess(in);
    if(dimension != num_neur*2){
        err("Dimension must be 558, which is double the number of neurons",
                "", "", FATAL_ERROR);
    }
    retrieve(beta, in["beta"], this);
    retrieve(memV, in["memV"], this);
    retrieve(memG, in["memG"], this);
    retrieve(EchemEx, in["EchemEx"], this);
    retrieve(EchemInh, in["EchemInh"], this);
    retrieve(ar, in["ar"], this);
    retrieve(ad, in["ad"], this);
    std::string ag_fname, a_fname;
    retrieve(ag_fname, in["ag_mat"], this);
    retrieve(a_fname, in["a_mat"], this);
    sparse_type ag_m(num_neur, num_neur);
    sparse_type a_m(num_neur, num_neur);
    laplacian.resize(num_neur, num_neur);
    read_mat(ag_fname, ag_m);
    read_mat(a_fname, a_m);
    double sumv = 0;
    for (int k=0; k<ag_m.outerSize(); ++k){
        for (SparseMatrix<double>::InnerIterator it(ag_m,k); it; ++it)
        {
            sumv += it.value();
        }
    }
    sparse_type temp(num_neur, num_neur);
    std::vector<Triplet<double, int> > temp_tr;
    for(int i = 0; i < (int)num_neur; i++){
        temp_tr.push_back(Triplet<double, int>(i, i, sumv));
    }
    temp.setFromTriplets(temp_tr.begin(), temp_tr.end());
    laplacian = temp - ag_m;
    AEchem_trans.resize(num_neur, num_neur);
    AEchem_trans = a_m.transpose();
    //will add rest later, this is just for performance test
    for(size_t i = 0; i < num_neur; i++){
        Echem[i] = EchemEx;
        vmean[i] = EchemInh;
    }
    for(size_t i = 0; i < num_neur; i++){
        if(GABAergic[i]){
            Echem[i] = EchemInh;
        }
        else{
            Echem[i] = EchemEx;
        }
    }
    sig.Constant(0.5);
    Matrix<double, num_neur, 1> eqS;
    eqS.array() = sig * (ar/(ar*sig + ad));
    Matrix<double, Dynamic, Dynamic> ldense(num_neur,num_neur);
    ldense = laplacian*Matrix<double, num_neur, num_neur>::Identity();
    Matrix<double, Dynamic, Dynamic> aedense(num_neur, num_neur);
    aedense= AEchem_trans*Matrix<double, num_neur, num_neur>::Identity();
    Matrix<double, Dynamic, Dynamic> C(num_neur, num_neur);
    C= memG*Matrix<double, num_neur, num_neur>::Identity() + gelec*ldense;
    Matrix<double, num_neur, 1> tmp =(gchem * aedense * eqS); 
    for(size_t i = 0; i < num_neur; i++){
        C(i, i) += tmp(i);
    }

    Matrix<double, num_neur, 1> b;
    b= gchem*aedense*(eqS.array() * Echem).matrix();
    for(size_t i = 0; i < num_neur; i++){
        b(i) += (1+memG*memV);
    }
    Matrix<double, num_neur, 1> eqV = C.inverse()*b;
    vmean = eqV.array()+1.0/(beta * (1.0/sig - 1).log());
}


std::vector<std::string> c_elegans::dependencies() const{
    std::string deps[] = {"beta", "memV", "memG", "gchem", "gelec",
        "tau", "EchemEx", "EchemInh", "ar", "ad", "ag_mat", "a_mat"};
    return make_append(deps, rhs_sde::dependencies());
}
