#ifndef CNLS_PULSE_H
#define CNLS_PULSE_H
#include "objective.h"
class bi_pulse_score:public objective{
    double* restr help;
    size_t nts;
    comp* restr kurtosis_help;
    fftw_plan ffor;
    public:
    virtual std::string type() const;
    virtual double score(comp* invals);
    virtual std::vector<std::string> dependencies() const;
    virtual void postprocess(std::map<std::string, item*>& inval);
    ~bi_pulse_score();
};
#endif
