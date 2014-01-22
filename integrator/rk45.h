#include "integrator.h"
#include "../utils/defs.h"
class rk45:public integrator{
    double dt_init, dt_min, dt_max, relerr, abserr;
    comp* restr f0, * restr f1, * restr f2, * restr f3;
    comp* restr f4, * restr f5, * restr f6;
    comp* restr u_calc;
    public:
    virtual void postprocess(std::map<std::string, item*>& dat);
    virtual void parse(std::string inval);
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual int integrate(rhs* rh, comp* restr u, double t0, double tf);
    virtual ~rk45();
};
