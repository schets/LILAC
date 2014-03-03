#ifndef CNLS_H
#define CNLS_H
#include "integrator/integrator.h"
#include "rhs/rhs.h"
#include "controller/controller.h"
#include "objective/objective.h"
#include "defs.h"
#include "stable.h"
#include <list>
//!jones matrix class, inherits from real8 since there is no meaningful functions for most of it
class jones_matrix;
class jones_optical:public stable_spectral_pde_1d{
    double* help, *t, *phold;
    comp* nvec1;
    double *nvec2;
    double best_score;
    double ba1, ba2, bap, ba3;
    comp* kurtosis_help;
    std::vector<jones_matrix*> jones_matrices;
    struct data_store{
        std::vector<double> avals;
        double score;
    };
    double jones_int_dist;
    std::list<data_store> out_dat;
    FILE* func_dat, *func_score;
    protected:
    virtual void pre_fft_operations();
    virtual void post_ifft_operations();
    virtual double get_change();
    public:
    virtual double score();
    virtual void postprocess(std::map<std::string, item*>& invals);
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual ~jones_optical();
};
#endif
