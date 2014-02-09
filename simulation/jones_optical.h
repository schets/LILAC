#ifndef CNLS_H
#define CNLS_H
#include "integrator/integrator.h"
#include "rhs/rhs.h"
#include "controller/controller.h"
#include "objective/objective.h"
#include "defs.h"
#include "stable.h"
//!jones matrix class, inherits from real8 since there is no meaningful functions for most of it
class jones_matrix;
class jones_optical:public stable_ode{
    double* help, *t, *phold;
    double best_score;
    comp* kurtosis_help;
    size_t nts;
    fftw_plan ffor, fback;
    std::vector<jones_matrix*> jones_matrices;
    protected:
    virtual void pre_integration_operations();
    virtual void post_integration_operations();
    virtual double get_change();
    public:
    virtual double score();
    virtual void postprocess(std::map<std::string, item*>& invals);
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual ~jones_optical();
};
#endif
