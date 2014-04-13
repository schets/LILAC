#ifndef RHS_SDE_H
#define RHS_SDE_H
#include "rhs.h"
class rhs_sde: public rhs{
    public:
        std::vector<std::string> dependencies() const;
        void postprocess(input& dat);
        /*!\brief
         * Stores the stochastic portion of the derivative in dw (calculates b(x) in dx = a(x)dt + b(x)dw
         */
        virtual int dwdt(void* x, void* dw, double t) = 0;
        virtual ~rhs_sde(){}
};
#endif
