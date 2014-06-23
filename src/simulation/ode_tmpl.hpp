/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
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
        std::cout << tcur << std::endl;
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
    in.retrieve(inter, "integrator", this);
    if(!inter->compare<T>()){
        err(this->name() + " is of type " + this->vname() + ", while the integrator inter \
                is of type " + inter->vname(), "ode_tmpl::postprocess", "simulation/ode_tmpl.hpp",
                FATAL_ERROR);
    }
    this->add_as_parent(inter);
    in.retrieve(tf, "tf", this);
    in.retrieve(t0, "t0", this, 0.0);
    if(t0 >= tf){
        err("t0 is >= than tf", "ode_tmpl::postprocess",
                "simulation/ode_tmpl.hpp", FATAL_ERROR);
    }

    in.retrieve(tw, "tw", this, t0);
    in.retrieve(w_step, "w_step",this, -1.0);
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
