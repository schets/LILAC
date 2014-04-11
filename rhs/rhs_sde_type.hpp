#ifndef RHS_SDE_TYPE_HPP
#define RHS_SDE_TYPE_HPP
#include "rhs_sde.h"
template<typename T> class rhs_type_sde:public rhs_sde{
    public:
    const std::type_info& vtype() const final{
        return typeid(T);
    }
    virtual void postprocess(std::map<std::string, item*>& dat){
        rhs_type<T>::postprocess(dat);
    }
    int dwdt(void* x, void* dw, double t) final{
        return dwdt((T* restr)x, (T* restr)dw, t);
    }
    virtual int dwdt(T* restr x, T* restr dw, double t) = 0;

    int dxdt(void* x, void* dx, double t) final{
        return dxdt((T* restr)x, (T* restr)dx, t);
    }
    virtual int dxdt(T* restr x, T* restr dx, double t) = 0;
};
#endif
