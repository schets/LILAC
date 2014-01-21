#ifndef INT_IMP_H
#define INT_IMP_H
#include "integrator.h"
#include <vector>
class rk4:public integrator{
    double stepsize;
    comp* restr f0, * restr f2, * restr f3, * restr f1;
    comp* restr u_calc;
    public:
    virtual int integrate(rhs* rh, comp* u, double t0, double tf);
    virtual std::vector<std::string> dependencies() const;
    virtual void postprocess(std::map<std::string, item*>& indat);
    virtual void retrieve(void* p) const;
    virtual void parse(std::string in);
    virtual std::string type()const;
    ~rk4();
};
#endif
