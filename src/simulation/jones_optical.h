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
#ifndef CNLS_H
#define CNLS_H
#include "integrator/integrator.h"
#include "controller/controller.h"
#include "stable_spectral_pde_1d_tmpl.hpp"
//!jones matrix class, inherits from real8 since there is no meaningful functions for most of it
class jones_matrix;
class jones_optical:public stable_spectral_pde_1d_tmpl<comp>{
    double* help, *t, *phold;
    comp* nvec1;
    double *nvec2;
    comp* kurtosis_help;
    std::vector<std::shared_ptr<jones_matrix> > jones_matrices;
    struct data_store{
        std::vector<double> avals;
        double score;
    };
    double jones_int_dist;
    std::list<data_store> out_dat;
    FILE* func_dat, *func_score;
    protected:
    virtual void pre_fft_operations();
    virtual void post_ifft_operations();
    virtual double get_change();
    public:
    virtual double score();
    virtual void postprocess(input& invals);
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual ~jones_optical();
};
#endif
