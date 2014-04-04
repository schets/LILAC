#ifndef STABLE_ODE_TMPL_HPP
#define STABLE_ODE_TMPL_HPP
#include "simulation.h"
template<class T>
class stable_ode_tmpl:public stable_ode{
    protected:
        double tcur;
        T* restr ucur, * restr ulast;
        //!This is the length of integration for each step
        double int_len;
        //!This is the starting time
        double t0; 
        //!This defines the integrator being used
        integrator* inter;
        //!This applies operations before the integration occurs
        virtual void pre_integration_operations();
        //!This applies operations ofter the integration occurs
        virtual void post_integration_operations();
        virtual void iterate_system();
        virtual double get_change();
    public:
        double score();
        const std::type_info& vtype() const;
        virtual void postprocess(std::map<std::string, item*>& invals);
        virtual std::string type() const;
        virtual ~stable_ode_tmpl();
};


//stable_ode_tmpl functions

/*!
 * This is the function that iterates the ODE system forwards.
 * It applies the operator pre_integration_operations, then integrates
 * from tcur to tcur+tlen, and then applies the operator post_integration_operations.
 * \sa stable::simulate
 */
template <class T>
void stable_ode_tmpl<T>::iterate_system(){
    for(size_t i = 0; i < dimension; i++){
        ulast[i] = ucur[i];
    }
    this->pre_integration_operations(); 
    int res = inter->integrate(ucur, tcur, tcur+int_len);
    tcur += int_len;
    this->post_integration_operations();
    if(res < 0){
        bad_res=1;
    }
}

template <class T>
void stable_ode_tmpl<T>::postprocess(std::map<std::string, item*>& invals){
    stable::postprocess(invals);
    invals["integrator"]->retrieve(&inter, this);
    invals["t0"]->retrieve(&t0, this);
    tcur = t0;
    invals["int_len"]->retrieve(&int_len, this);
    const double eps_val = 1e-12;
    if(int_len < eps_val){
        err("Variable int_len is too small, int_len must be greater than 1e-12",
                "stable_ode::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
    ucur = (T*)al_malloc(2*dimension*sizeof(T));
    ulast=ucur+dimension;
}

template <class T>
stable_ode_tmpl<T>::~stable_ode_tmpl(){
    al_free(ucur);
}
template <class T>
void stable_ode_tmpl<T>::pre_integration_operations(){
}
template <class T>
void stable_ode_tmpl<T>::post_integration_operations(){
}
template <class T>
std::string stable_ode_tmpl<T>::type() const{
    std::string val = "stable_ode_tmpl";
    val.append(typeid(T).name());
    return val;
}
template <class T>
double stable_ode_tmpl<T>::score(){
    return obj->score((comp*)ucur);
}

template<class T>
const std::type_info& stable_ode_tmpl<T>::vtype() const{
    return typeid(T);
}
template<class T>
double stable_ode_tmpl<T>::get_change(){
    //standard difference is the L2 norm
    double sabs=0;
    for(size_t i = 0; i < dimension; i++){
        double aval = abs(ucur[i]-ulast[i]);
        sabs+= aval*aval;
    }
    return sqrt(sabs);
}
#endif
