#pragma once
#ifndef ODE_HPP
#define ODE_HPP
#include "ode.h"
#include "utils/comp_funcs.hpp"
#include "utils/item_heads.hpp"
#include "writer/writer.h"
#include "engine/engineimp.h"
#include <limits>
#include <time.h>
template<class T>
class ode_tmpl:public ode{
    void integrate_with_writes();
    void write_t(double tcur);
    //!integrator currently being used
    //!Vector representing the current solution
    T* sol;
    //!Starting time for the integration. By default is equal to zero
    double t0;
    //!Ending time for the integration
    double tf;
    //!Time at which data recording begins. By default is equal to t0.
    double tw;
    //!Interval at which data is written. By default is equal to -1
    double w_step;
    public:
    std::string type() const;
    const std::type_info& vtype() const;
    void postprocess(input& in);
    double simulate();
};
template<class T>
void ode_tmpl<T>::write_t(double tcur){
    std::shared_ptr<writer> dat_writer=std::shared_ptr<writer>(new writer(true));
    dat_writer->add_data(
            data::create("Time", tcur), writer::INTERMEDIATE_SCORE);
    dat_writer->add_data(
            data::create("Func", sol, dimension), writer::INTERMEDIATE_FUNC);
    holder->add_writer(dat_writer);
}
template<class T>
void ode_tmpl<T>::integrate_with_writes(){
    double tcur = tw;
    while(tcur <= (tf-w_step)){
        write_t(tcur);
        inter->integrate(sol, tcur, tcur + w_step);
        tcur += w_step;
    }
    write_t(tcur);
    inter->integrate(sol, tcur, tf);
}

template<class T>
void ode_tmpl<T>::postprocess(input& in){
    simulation::postprocess(in);
    item_dim::postprocess(in);
    retrieve(inter, in["integrator"], this);
    if(!inter->compare<T>()){
        err(this->name() + " is of type " + this->vname() + ", while the integrator inter \
                is of type " + inter->vname(), "ode_tmpl::postprocess", "simulation/ode_tmpl.hpp",
                FATAL_ERROR);
    }
    this->add_as_parent(inter);
    retrieve(tf, in["tf"], this);
    retrieve(t0, in["t0"], this, 0.0);
    if(t0 >= tf){
        err("t0 is >= than tf", "ode_tmpl::postprocess",
                "simulation/ode_tmpl.hpp", FATAL_ERROR);
    }

    retrieve(tw, in["tw"], this, t0);
    retrieve(w_step, in["w_step"],this, -1.0);
    if(tw >= tf){
        err("tw is >= than tf, not writing intermittent data", "ode_tmpl::postprocess",
                "simulation/ode_tmpl.hpp", WARNING);
        w_step=-1;
    }
    if(tw < t0){
        tw = t0;
    }
    this->memp.create(this->dimension, &sol);
}
template<class T>
double ode_tmpl<T>::simulate(){
    inter->initial_condition(sol);
    clock_t cval = clock();
    if(w_step < 0 || abs(w_step) <= std::numeric_limits<double>::epsilon()){
        //don't do any intermittent recording
        inter->integrate(sol, t0, tf);
    }
    else{
        if(tw > t0){
            inter->integrate(sol, t0, tw);
        }
        integrate_with_writes();
    }
    write_t(tf);
    std::cout << "Simulation took " << ((double)(clock()-cval))/CLOCKS_PER_SEC << " seconds" << std::endl;
    return 0;
}
template<class T>
std::string ode_tmpl<T>::type() const{
    return "ode_tmpl<"+this->vname() + ">";
}
template<class T>
const std::type_info& ode_tmpl<T>::vtype() const{
    return typeid(T);
}
#endif
