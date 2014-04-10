#ifndef RHS_TYPE_H
#define RHS_TYPE_H
#include "rhs.h"
//!Type interface for the rhs class
/*!
 *
 * This class associates each rhs object with a given type
 * This make it possible to allow for runtime selection of typed integrators
 * While using a template to help save code. Each integrator should always support
 * the comp, float, and double variable types. This should be enough for almost all
 * cases but you can define your own if need be
 *
 * \sa rk45, \sa rhs, \sa rhs_CNLS, \sa rk45, \sa rk45_tmpl
 */
template<typename T> class rhs_type:public rhs{
    public:
    virtual std::string val_type() const{
        return typeid(T).name();
    }
    virtual std::vector<std::string> dependencies() const{
        return rhs::dependencies();
    }
    void postprocess(std::map<std::string, item*>& dat){
        rhs::postprocess(dat);
    }
    inline int dxdt(void* restr x, void* restr dx, double t){
        return dxdt((T* restr)x, (T* restr)dx, t);
    }
    virtual int dxdt(T* restr x, T* restr dx, double t) = 0;
};
#endif