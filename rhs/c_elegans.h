#ifndef C_ELEGANS_H
#define C_ELEGANS_H
#include "rhs_sde_type.hpp"
#include "eigen3/Eigen/Eigen"
#include "eigen3/Eigen/Sparse"
using namespace Eigen;
class c_elegans:public rhs_sde_type<double>{
    Eigen::SparseMatrix<double, RowMajor> laplacian, AEchem_trans;
    Matrix<double, Dynamic, 1> Ielec, Ichem, Iohm, Echem;
    double* restr vmean, * restr sig;
    size_t dim_v;
    double beta, memV, memG, gchem, gelec, tau;
    std::vector<size_t> inj_nodes;
    double cur_inj;
    public:
    std::string type() const;
    void postprocess(input& in);
    int dxdt(double* restr x, double* restr dx, double dt);
    int dwdt(double* restr x, double* restr dx, double dt);
};
#endif
