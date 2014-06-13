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
        virtual int dwdt(ptr_passer x, ptr_passer dw, double t) = 0;
        //!Performs modifications on the noise vector
        /*
         * Allows the rhs to perform operations on the noise vector. This could be desired for
         * effeciency purposes, since a function may want to have no noise in some portions,
         * without the overhead of creating a fake dw
         */
        virtual void mod_w(ptr_passer w, double t){};
        virtual ~rhs_sde(){}
};
#endif
