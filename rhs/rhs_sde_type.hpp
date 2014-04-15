#ifndef RHS_SDE_TYPE_HPP
#define RHS_SDE_TYPE_HPP
#include "rhs_sde.h"
template<typename T> class rhs_sde_type:public rhs_sde{
    public:
    const std::type_info& vtype() const final{
        return typeid(T);
    }
    virtual void postprocess(input& dat) = 0;
    int dwdt(void* x, void* dw, double t) final{
        return dwdt((T* restr)x, (T* restr)dw, t);
    }
    virtual int dwdt(T* restr x, T* restr dw, double t) = 0;

    int dxdt(void* x, void* dx, double t) final{
        return dxdt((T* restr)x, (T* restr)dx, t);
    }
    virtual int dxdt(T* restr x, T* restr dx, double t) = 0;
};


    template<class T>
void rhs_sde_type<T>::postprocess(input& dat)
{
    rhs_sde::postprocess(dat);
}
#endif
