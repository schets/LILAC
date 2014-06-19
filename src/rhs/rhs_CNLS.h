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
#ifndef RHS_IMP_H
#define RHS_IMP_H
#include "rhs_type.hpp"
//!The class defining the CNLS equations
/*!
 * This class provides an implementation of the coupled nonlinear schrodinger equations
 * SHOULD ADD ACTUAL EQUATIONS
 */
class rhs_CNLS: public rhs_type<comp>{
    //This class defines the rhs equations for the coupled CNLS
    comp * restr u1, * restr u2, * restr uf1, *restr uf2, * restr comp_in,
         * restr comp_out, * restr comp_out_r, * restr comp_in_r;
    double * restr sq1, * restr sq2,  * restr ksq;
    double g0;
    double e0;
    double dt;
    double LENGTH_T;
    size_t NUM_TIME_STEPS;
    public:
    double* restr k;
    virtual std::vector<std::string> dependencies() const;
    virtual  std::string type() const;
    virtual void postprocess(input& dat);
    ~rhs_CNLS();
    virtual int dxdt(ptr_passer x, ptr_passer dx, double t);
};
#endif
