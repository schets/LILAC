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
#include "rhs_CNLS.h"
#include "comp_funcs.hpp"
#include "utils/ptr_passer.hpp"
#include "types/type_register.hpp"

template class type_register<rhs_CNLS>;
/*!
 * Destructor for rhs_CNLS
 * */
rhs_CNLS::~rhs_CNLS(){}
    
int rhs_CNLS::dxdt(ptr_passer x, ptr_passer _dx, double t){
    comp* restr dx = _dx;
    uf1= x;
    uf2=uf1+NUM_TIME_STEPS;
    //take the inverse fourier transform
    ifft(uf1, u1, NUM_TIME_STEPS);
    ifft(uf2, u2, NUM_TIME_STEPS);
    //Inform compiler of alignment, if supported
    ALIGNED(uf1);
    ALIGNED(uf2);
    ALIGNED(u1);
    ALIGNED(u2);
    ALIGNED(sq1);
    ALIGNED(sq2);
    ALIGNED(comp_in_r);
    ALIGNED(comp_in);
    ALIGNED(comp_out);
    ALIGNED(comp_out_r);
    ALIGNED(k);
    ALIGNED(ksq);

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        sq1[i] = _sqabs(u1[i]);
        sq2[i] = _sqabs(u2[i]);
        comp_in_r[i] = sq1[i] + sq2[i];
    }
    comp expr1 = Id*(2.0*g0/(1.0+trap(comp_in_r, NUM_TIME_STEPS)*dt/e0));
    //calculate the ffts for the rhs

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i] = (sq1[i] + A*sq2[i])*u1[i];
        comp_in[i] = u2[i] * u2[i] * _conj(u1[i]);
    }
    //fourier transform forwards nonlinear equations
    fft(comp_in, comp_out, NUM_TIME_STEPS);
    fft(comp_in_r, comp_out_r, NUM_TIME_STEPS);

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i] = (((D/2) * ksq[i] + K) * uf1[i] - comp_out_r[i]
                - B*comp_out[i] + expr1*uf1[i]*(1-tau*ksq[i]) - Gamma*uf1[i])/Id;
    }
    //Do the fft work for the other half of the calculations

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i] = (A*sq1[i] + sq2[i])*u2[i];
        comp_in[i] = u1[i] * u1[i] * _conj(u2[i]);
    }
    fft(comp_in, comp_out, NUM_TIME_STEPS);
    fft(comp_in_r, comp_out_r, NUM_TIME_STEPS);

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i+NUM_TIME_STEPS] = (((D/2) * ksq[i] - K) * uf2[i] - comp_out_r[i]
                - B*comp_out[i] + expr1*(uf2[i]-tau*ksq[i]*uf2[i]) - Gamma*uf2[i])/Id;
    }
    return 0;
}

std::vector<std::string> rhs_CNLS::dependencies() const{
    std::string deps[] = {"g0", "e0", "t_int"};
    return make_append(deps, rhs::dependencies());
}

std::string rhs_CNLS::type() const {
    return "CNLS";
}
/*!
 * Initializes the rhs_CNLS class  
 */
void rhs_CNLS::postprocess(input& dat){
    rhs::postprocess(dat);
    NUM_TIME_STEPS = dimension/2;
    if(NUM_TIME_STEPS*2 != dimension){
        err("dimension not even, which is required for rhs_CNLS", 
                "rhs_CNLS::postprocess", "rhs/rhs_CNLS.cpp", FATAL_ERROR);
    }
    dat.retrieve(LENGTH_T, "t_int", this);
    if(LENGTH_T <= 0){
        std::string errmess = "t_int is invalid, must be >= 0";
        err(errmess, "rhs_CNLS::postprocess", "rhs/rhs_CNLS.cpp",
                dat["t_int"], FATAL_ERROR);
    }
    dt = LENGTH_T/NUM_TIME_STEPS;
    dat.retrieve(g0, "g0", this);
    dat.retrieve(e0, "e0", this);
    memp.create(NUM_TIME_STEPS, &u1, &u2, &comp_in, &comp_in_r, &comp_out, &comp_out_r, &sq1, &sq2, &k, &ksq);
    //create k values
    double mulval=(2.0*PI/LENGTH_T)*(NUM_TIME_STEPS/2.0);
    for(size_t i=0; i<NUM_TIME_STEPS/2; i++){
        k[i] = mulval * (2.0*i/(1.0*NUM_TIME_STEPS));
        ksq[i] = k[i]*k[i];
    }
    for(size_t i=NUM_TIME_STEPS/2; i<NUM_TIME_STEPS; i++){
        k[i] = mulval * 2.0*((int)i-(int)NUM_TIME_STEPS)/(NUM_TIME_STEPS*1.0);
        ksq[i] = k[i]*k[i];
    }

}
