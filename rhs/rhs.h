#ifndef RHS_H
#define RHS_H
#include "defs.h"
//input pointers are not constant as they are used in the FFT routines

class rhs{
    protected:
        size_t dimension;
    public:
        inline size_t dim() const{
            return dimension;
        }
        rhs(size_t dimen): dimension(dimen){}
        virtual int dxdt(comp* __restrict__ x, comp* __restrict__ dx, double t) = 0;
        virtual ~rhs(){}
};
class rhs_x:public rhs{
    public:
        rhs_x(size_t dimen): rhs(dimen){}
        virtual int dxdt(comp* __restrict__ x, comp* __restrict__ dx, double t);
};
class rhs_const:public rhs{ 
    const comp const_val;
    public:
    rhs_const(comp cval, size_t dimen):const_val(cval), rhs(dimen){
    }
    virtual int dxdt(comp* __restrict__ x, comp* __restrict__ dx, double t);
};
class rhs_CNLS: rhs{
    //this is temporary, to make porting existing C code easier
    odeparams* p;
    public:
        virtual int dxdt(comp* __restrict__ x, comp* __restrict__ dx, double t);
};
#endif
