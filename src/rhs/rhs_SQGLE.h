#include "rhs_type.hpp"

class rhs_SQGLE: public rhs_type<comp>{
    //This class defines the rhs equations for the coupled CNLS
    comp * restr u1, * restr u2, * restr uf1, * restr comp_in,
         * restr comp_out,  * restr comp_in_r;
    double * restr sq1, * restr ksq;
    double a1, a2, a3, ap;
    double g0;
    double e0;
    double dt;
    double LENGTH_T;
    size_t NUM_TIME_STEPS;
    public:
    void update();
    double* restr k;
    virtual void parse(std::string inval);
    virtual std::vector<std::string> dependencies() const;
    virtual  std::string type() const;
    virtual void postprocess(input& dat);
   // virtual void parse(std::string inval);
  //  virtual void retrieve(void* inval) const ;
    ~rhs_SQGLE();
    virtual int dxdt(ptr_passer x, ptr_passer dx, double t);
};
