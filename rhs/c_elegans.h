#ifndef C_ELEGANS_H
#define C_ELEGANS_H
#include "rhs_sde_type.hpp"
#include "eigen3/Eigen/Eigen"
#include "eigen3/Eigen/Sparse"
using namespace Eigen;
class c_elegans:public rhs_sde_type<double>{
    constexpr static size_t num_neur = 279;
    typedef Eigen::SparseMatrix<double, RowMajor> sparse_type;
    sparse_type laplacian, AEchem_trans;
    Matrix<double, num_neur, 1> Ielec, Ichem, Iohm, Echem;
    double* restr vmean, * restr sig;
    constexpr static size_t dim_v = num_neur;
    constexpr static size_t dim_s = dim_v;
    double beta, memV, memG, gchem, gelec, tau, EchemEx, EchemInh, ar, ad;
    std::vector<size_t> inj_nodes;
    double cur_inj;
    public:
    std::string type() const;
    void postprocess(input& in);
    std::vector<std::string> dependencies() const;
    int dxdt(double* restr x, double* restr dx, double dt);
    int dwdt(double* restr x, double* restr dx, double dt);
};
#endif
