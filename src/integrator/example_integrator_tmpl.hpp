#ifndef example_integrator
#define example_integrator

//It seems that it would be easier to use dynamic types, since we have a problem
//where the type is decided at runtime. However, that murders performance terribly
//so we do compile-time type selection and then instantiate the right type at runtime from 
//a list of types with the type_constructor class
template<class T>
class example_integrator_tmpl : public example_integrator{
    double rval1, calc_val;
    unsigned unsigned_var;
    int rval2;
    std::string something;
    public:
    //!Actual implementation of the integration function
    int integrate(ptr_passer u, double t0, double tf);
    std::string type() const;
    const std::type_info& vtype() const{
        //This gives the type ofthe template
        return typeid(T);
    }
    void postprocess(input& in);
};

template<class T>

#endif
