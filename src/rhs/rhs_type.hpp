#ifndef RHS_TYPE_H
#define RHS_TYPE_H
#include "rhs.h"
//!Type interface for the rhs class
/*!
 *
 * This class associates each rhs object with a given type
 * This make it possible to allow for runtime selection of typed integrators
 * while using a template to help save code. Each general purpose integrator should always support
 * a type that has the basic numeric operators and an abs function defined.
 * This should be enough for almost all
 * cases but you can define your own if need be
 *
 * \sa rk45, \sa rhs, \sa rhs_CNLS, \sa rk45, \sa rk45_tmpl
 */
template<typename T> class rhs_type:public rhs{
    public:
        //!returns the type of the current variable
        virtual const std::type_info& vtype() const final{
            return typeid(T);
        }
        
        //!Generic initial condition, call default constructor for each element/
        virtual void initial_condition(ptr_passer in, size_t len){
//ICC doesn't support default_constructible
#ifndef ICC
            static_assert(std::is_default_constructible<T>::value,
                    "Type needs to be default constructable in rhs_type");
#endif
            T* vals = in.get<T>();
            for(size_t j = 0; j < len; j++){
                vals[j] = T();
            } 
        }
};
#endif
