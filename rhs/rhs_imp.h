#ifndef RHS_IMP_H
#define RHS_IMP_H
#include "rhs.h"

class rhs_const:public rhs{
    //test class for x'=const
    comp const_val;
    public:
    virtual std::vector<std::string> dependencies() const;
    virtual  std::string type() const;
    //virtual void parse(std::string inval);
    //virtual void retrieve(void* inval) const ;
    rhs_const():const_val(0){};
    rhs_const(comp cval, size_t dimen):const_val(cval), rhs(dimen){
    }
    virtual int dxdt(comp* restr x, comp* restr dx, double t);
};
class rhs_CNLS: public rhs{
    //This class defines the rhs equations for the coupled CNLS
    comp * restr u1, * restr u2, * restr uf1, *restr uf2, * restr comp_in,
         * restr comp_out, * restr comp_out_r, * restr comp_in_r;
    double * restr sq1, * restr sq2, * restr k, * restr ksq;
    fftw_plan ffor, fback;
    double g0, e0, dt, LENGTH_T;
    size_t NUM_TIME_STEPS;
    public:
    virtual std::vector<std::string> dependencies() const;
    virtual  std::string type() const;
   // virtual void parse(std::string inval);
  //  virtual void retrieve(void* inval) const ;
    rhs_CNLS(){};
    rhs_CNLS(size_t dimen, double g, double e, double dt, double tlen, size_t n_steps);
    ~rhs_CNLS();
    virtual int dxdt(comp* restr x, comp* restr dx, double t);
};
#endif
