#include "c_elegans.h"
#include "engine/input.h"
#include "utils/comp_funcs.hpp"
#include "utils/item_heads.hpp"
#include <vector>
#include <fstream>
#include "gaba_list.hpp"
#include <eigen3/Eigen/Core>
#include "ptr_passer.hpp"
constexpr size_t c_elegans::num_neur;
constexpr size_t c_elegans::dim_v;
constexpr size_t c_elegans::dim_s;
int c_elegans::dxdt(ptr_passer x,  ptr_passer dx, double dt){
    double* restr v = x.get<double>();
    double* restr dv = dx.get<double>();
    double* restr ds = dv+dim_v;
    double* restr s = v+dim_v;
    Map<Array<double, dim_v, 1>> vmap(v);
    Map<Array<double, dim_v, 1>> dvmap(dv);
    Map<Array<double, dim_v, 1>> smap(s);
    Map<Array<double, dim_v, 1>> dsmap(ds);

    sig = 1.0 / (1.0 + (-1*beta*(vmap-vmean)).exp());
    dsmap = ar*(sig * (1.0-smap)) - ad*smap;
    Iohm = (memG*(vmap - memV));
    Ielec = (gelec*laplacian*vmap.matrix()).array();
    Ichem = (gchem *
            (vmap*(AEchem_trans*smap.matrix()).array() 
             - (AEchem_trans*(smap*Echem).matrix()).array()));
   /* dvmap = (-1.0/tau)*((memG*(vmap - memV))
            +(gelec*laplacian*vmap.matrix()).array()+
            (gchem *(vmap*(AEchem_trans*smap.matrix()).array()
                     - (AEchem_trans*(smap*Echem).matrix()).array())));*/
    dvmap = (-1.0/tau)*(Iohm + Ielec + Ichem);
    for(auto s : inj_nodes){
        dv[s] += (-1.0/tau)*cur_inj;
    }
    double amp = 2e4;
    dv[276]+= (-1.0/tau)*amp;
    dv[278]+= (-1.0/tau)*amp;
    err("", "", "", FATAL_ERROR);
    return 0;
}
int c_elegans::dwdt(ptr_passer x, ptr_passer _dx, double dt){
    double* dx = _dx.get<double>();
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
    rhs_type::postprocess(in);
    if(dimension != num_neur*2){
        err("Dimension must be 558, which is double the number of neurons",
                "", "", FATAL_ERROR);
    }
    retrieve(beta, in["beta"], this);
    retrieve(tau, in["tau"], this);
    retrieve(gelec, in["gelec"], this);
    retrieve(gchem, in["gchem"], this);
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
    Matrix<double, Dynamic, Dynamic> ag_dense(num_neur, num_neur);
    ag_dense = ag_m *  Matrix<double, num_neur, num_neur>::Identity();
    auto sumvals = ag_dense.colwise().sum();
    sparse_type temp(num_neur, num_neur);
    std::vector<Triplet<double, int> > temp_tr;
    for(int i = 0; i < (int)num_neur; i++){
        temp_tr.push_back(Triplet<double, int>(i, i, sumvals[i]));
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
    for(size_t i = 0; i < num_neur; i++){
        sig[i] = 0.5;
    }
    
    eqS = sig * (ar/(ar*sig + ad));
    Matrix<double, Dynamic, Dynamic> ldense(num_neur,num_neur);
    ldense = laplacian*Matrix<double, num_neur, num_neur>::Identity();
    Matrix<double, Dynamic, Dynamic> aedense(num_neur, num_neur);
    aedense= AEchem_trans*Matrix<double, num_neur, num_neur>::Identity();
    Matrix<double, Dynamic, Dynamic> C(num_neur, num_neur);
    C= memG*Matrix<double, num_neur, num_neur>::Identity() + gelec*ldense;
    Matrix<double, num_neur, 1> tmp =(gchem * aedense * eqS.matrix()); 
    for(size_t i = 0; i < num_neur; i++){
        C(i, i) += tmp(i);
    }
    std::cout << C.colwise().sum() << std::endl;
    Matrix<double, num_neur, 1> Ivals;
    Ivals.setZero();
    double amp=2e4;
    Ivals[276]=amp;
    Ivals[278]=amp;
    Matrix<double, num_neur, 1> b;
    b= gchem*aedense*(eqS * Echem).matrix();
    for(size_t i = 0; i < num_neur; i++){
        b[i] += (memG*memV + Ivals[i]);
    }
    eqV.matrix() = C.inverse()*b;
    vmean = eqV+(1.0/beta) * (1.0/sig - 1).log();
}


std::vector<std::string> c_elegans::dependencies() const{
    std::string deps[] = {"beta", "memV", "memG", "gchem", "gelec",
        "tau", "EchemEx", "EchemInh", "ar", "ad", "ag_mat", "a_mat"};
    return make_append(deps, rhs_type::dependencies());
}

void c_elegans::initial_condition(ptr_passer in, size_t len){
    if(len != dimension){
        rhs_type<double>::initial_condition(in, len);
    }
    double* vals = in.get<double>();
    for(size_t i = 0; i < num_neur; i++){
        vals[i] = eqV[i];
        vals[i+dim_v] = eqS[i];
        vals[i] = 0.1;
        vals[i+dim_v]=0.1;
    }
}
