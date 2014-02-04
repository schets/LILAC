#ifndef CNLS_PULSE_H
#define CNLS_PULSE_H
#include "objective.h"
class cnls_pulse:public objective{
    double* restr help;
    public:
        virtual double score(comp* invals);
        virtual std::vector<std::string> dependencies() const{return std::vector<std::string>();};
        virtual void postprocess(std::map<std::string, item*>& inval){};
};
#endif
