#ifndef EXAMPLE_INTEGRATOR_TMPL_HPP
#define EXAMPLE_INTEGRATOR_TMPL_HPP
#include "example_integrator.h"
#include "types/float_traits.hpp"
//It seems that it would be easier to use dynamic types, since we have a problem
//where the type is decided at runtime. However, that murders performance terribly
//so we do compile-time type selection and then instantiate the right type at runtime from 
//a list of types with the type_constructor class
template<class T>
class example_integrator_tmpl : public example_integrator{
    //!The current type of floating point value
    /*!
    * This queries the float_traits template class to see
    * what the underlying real type is.
    * 
    * Why do this instead of use double everywhere?
    * If a program is using 32bit floats, then double take up extra memory
    * and waste lots of time in the conversion process.
    * If a program uses 80 or 128 bit floats, or higher, then doubles aren't
    * big enough. Hence, we use a type_traits class that gives us the proper type
    * for the variable being used. Any type that is parameterized by a floating_point type value
    * is automatically included in float_traits, and so are any floating point types.
    * For an example of how to define float_traits for a custom type, see example_type.
    * Also see the postprocess function for how to deal with this type in postprocessing
    * \sa example_integrator_tmpl::postprocess
    * \sa example_type
    * \sa float_traits
    */
    typedef typename float_traits<T>::type real_type;
    real_type rval1, calc_val;

    //Why don't we use the real_type to represent the integer types?
    //floating point values can be very, very bad at representing large integer numbers.
    size_t unsigned_var;
    int rval2;
    std::string something;
    public:
    //!Actual implementation of the integration function
    int integrate(ptr_passer u, double t0, double tf);
    std::string type() const{
        return std::string("euler_sde_tmpl<")+typeid(T).name()+">";
    }
    const std::type_info& vtype() const{
        //This gives the type ofthe template
        return typeid(T);
    }
    void postprocess(input& in);
    //This is called if a variable that this class references changes
    //Hence why this must be passed to each retireve call-so the engine knows who
    //needs updating
    //
    //Don't use this for IO utilities, and prefer to move logic into the controller class.
    //The c_elegans rhs class is a bad example, but it needed to be done right away and was quicker than
    //re-architecting parts of the engine for native mutable lists
    void update();
};

template<class T>
int example_integrator_tmpl<T>::integrate(ptr_passer _u, double t0, double tf){
    //First, get the actual pointer out of the holder
    //The holder helps to ensure type-safety
    //The restr declaration tells the compiler that nothing else points at this location
    //That is a lie!!! But nothing will be modifying this location during the function call
    //so it can be delcared restr. Even if this were multithreaded, it would be incorrect to access this pointer
    //from another thread and would have meaningless results anyways
    T* restr u = _u.get<T>();

    //This declaration will tell the compiler that the resulting pointer is aligned in memory.
    //If you use the mempool and align to 16 bytes or more (32, 64, etc) This will be true
    ALIGNED(u);

    //The input array will be of length dimension, unless you use a simulation class that doesn't
    //enforce dimension-correctness with the items that it holds.
    for(size_t i = 0; i < this->dimension; i++){
        //Some magical integration function
        u[i] = (rval1 * rval2) + (calc_val * unsigned_var) * u[i];
    }
    return 0;
}

template<class T>
void example_integrator_tmpl<T>::postprocess(input& in){
    //this allows us to have a consistent representation in the input file.
    //If the variable in question is not part of the hardcore numerical analysis code,
    //it may be better to just use a double and not deal with this in the postprocessing
    double _rval1 = 0; 
    retrieve(_rval1, in["rval1"], this);
    rval1=_rval1;
    retrieve(rval2, in["rval2"], this);
    retrieve(unsigned_var, in["unsigned_var"], this);
    retrieve(something, in["something"], this);
    update();
}

template<class T>
void example_integrator_tmpl<T>::update(){
    calc_val = rval1*3+rval2;
}
#endif
