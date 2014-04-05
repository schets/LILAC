#ifndef CNLS_PULSE_H
#define CNLS_PULSE_H
#include "objective.h"
class n_pulse_score:public objective{
    size_t n_pulse;
    double* restr help;
    size_t nts;
    comp* restr kurtosis_help;
    public:
    virtual std::string type() const;
    virtual double score(comp* invals);
    virtual std::vector<std::string> dependencies() const;
    virtual void postprocess(std::map<std::string, std::shared_ptr<item>>& inval);
    ~n_pulse_score();
};
#endif
