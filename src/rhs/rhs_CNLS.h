#ifndef RHS_IMP_H
#define RHS_IMP_H
#include "rhs_type.hpp"
//!The class defining the CNLS equations
/*!
 * This class provides an implementation of the coupled nonlinear schrodinger equations
 * SHOULD ADD ACTUAL EQUATIONS
 */
class rhs_CNLS: public rhs_type<comp>{
    //This class defines the rhs equations for the coupled CNLS
    comp * restr u1, * restr u2, * restr uf1, *restr uf2, * restr comp_in,
         * restr comp_out, * restr comp_out_r, * restr comp_in_r;
    double * restr sq1, * restr sq2,  * restr ksq;
    double g0;
    double e0;
    double dt;
    double LENGTH_T;
    size_t NUM_TIME_STEPS;
    public:
    double* restr k;
    virtual std::vector<std::string> dependencies() const;
    virtual  std::string type() const;
    virtual void postprocess(input& dat);
    ~rhs_CNLS();
    virtual int dxdt(ptr_passer x, ptr_passer dx, double t);
};
#endif
