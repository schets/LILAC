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
#ifndef STABLE_SPECTRAL_PDE_1D_TMPL_HPP
#define STABLE_SPECTRAL_PDE_1D_TMPL_HPP
//!This class represents the spectral integration of a PDE
/*!
 * This class will represent the integration of a 1d spectral ODE.
 * I'm not sure how to properly generalize it to multiple variables,
 * so I'll just leave it blank for now
 */
#include "stable_ode_tmpl.hpp"
#include "utils/comp_funcs.hpp"
template<class T>
class stable_spectral_pde_1d_tmpl:public stable_ode_tmpl<T>{
    private:
        virtual void pre_integration_operations();
        virtual void post_integration_operations();
    protected:
        double* t;
        double* help;
        size_t nts;
        size_t num_pulses;
        //!Applies operations prior to the fft and integration
        virtual void pre_fft_operations();
        //!Applies operations after the fft but before the integrations
        virtual void post_fft_operations();
        //!Applies operations after the integration but prior to the ifft
        virtual void pre_ifft_operations();
        //!Applies operations after the integration and ifft()
        virtual void post_ifft_operations();
    public:
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(input& invals);
        virtual std::string type() const;
        virtual ~stable_spectral_pde_1d_tmpl();
};

//stable_spectral_pde_1d functions
template<class T>
std::string stable_spectral_pde_1d_tmpl<T>::type() const{
    return std::string("stable_spectral_pde_1d_tmpl")+typeid(T).name();
}

template<class T>
std::vector<std::string> stable_spectral_pde_1d_tmpl<T>::dependencies() const{
    std::string deps[] = {"num_pulses"};
    return make_append(deps, stable_ode_tmpl<T>::dependencies());
};

template<class T>
void stable_spectral_pde_1d_tmpl<T>::postprocess(input& invals){
    stable_ode_tmpl<T>::postprocess(invals);
    int _num;
    invals.retrieve(_num, "num_pulses", this);
    if(_num <= 0){
        err("Number of pulses must be greater than zero", "stable_spectral_pde_1d_tmpl<T>::postprocess",
                "simulation/stable.cpp", FATAL_ERROR);
    }
    num_pulses=_num;
    if(this->dimension%num_pulses){
        err("Dimension must be divisible by the number of pulses",  "stable_spectral_pde_1d_tmpl<T>::postprocess",
                "simulation/stable.cpp", FATAL_ERROR);
    }
    nts=this->dimension/num_pulses;
    double dt=60.0/nts;
    this->memp.add(nts, &t, &help);
    for(size_t i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }
    for(size_t i = 0; i < nts; i++){
        this->ucur[i] = this->ucur[i+nts] = 1.00/cosh(t[i]/2.0);
        this->help[i] = _real(this->ucur[i]);
    }
}
template<class T>
void stable_spectral_pde_1d_tmpl<T>::pre_integration_operations(){
    if(stable::round==0){
        for(size_t i = 0; i < nts; i++){
            this->ucur[i] = this->ucur[i+nts] = 1.00/cosh(t[i]/2.0);
        }
    }
    pre_fft_operations();
    for(size_t i = 0; i < num_pulses; i++){
        fft(this->ucur+i*nts, this->ucur+i*nts, nts);
    }
    post_fft_operations();
}
template<class T>
void stable_spectral_pde_1d_tmpl<T>::post_integration_operations(){
    pre_ifft_operations();

    for(size_t i = 0; i < num_pulses; i++){
        ifft(this->ucur+i*nts, this->ucur+i*nts, nts);
    }
    post_ifft_operations();
}
template<class T>
void stable_spectral_pde_1d_tmpl<T>::pre_fft_operations(){};

template<class T>
void stable_spectral_pde_1d_tmpl<T>::post_fft_operations(){};

template<class T>
void stable_spectral_pde_1d_tmpl<T>::pre_ifft_operations(){};

template<class T>
void stable_spectral_pde_1d_tmpl<T>::post_ifft_operations(){};

template<class T>
stable_spectral_pde_1d_tmpl<T>::~stable_spectral_pde_1d_tmpl()
{
}
#endif
