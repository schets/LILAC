#ifndef C_ELEGANS_H
#define C_ELEGANS_H
#include "rhs_type.hpp"
#include "eigen3/Eigen/Eigen"
#include "eigen3/Eigen/Sparse"
using namespace Eigen;
class c_elegans:public rhs_type<double>{
    int cur_ind;
    double dummy;
    constexpr static size_t num_neur = 279;
    constexpr static size_t al_num = 4;
    typedef Eigen::SparseMatrix<double, RowMajor> sparse_type;
    sparse_type laplacian, AEchem_trans, AEchem_trans_full, ag_full;
    Array<double, num_neur, 1> vmean, sig, Echem, eqS, eqV;
    constexpr static size_t dim_v = num_neur;
    constexpr static size_t dim_s = dim_v;
    double beta, memV, memG, gchem, gelec, tau, EchemEx, EchemInh, ar, ad;
    std::vector<size_t> inj_nodes;
    std::vector<size_t> abl_neur;
    double cur_inj;
    public:
    virtual void update();
    std::string type() const;
    void postprocess(input& in);
    std::vector<std::string> dependencies() const;
    int dxdt(ptr_passer x, ptr_passer dx, double dt);
    int dwdt(ptr_passer x, ptr_passer dx, double dt);
    void initial_condition(ptr_passer in, size_t len);
};
#endif
