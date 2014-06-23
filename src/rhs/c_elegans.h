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
#ifndef C_ELEGANS_H
#define C_ELEGANS_H
#include "rhs_type.hpp"
#include "rhs_sde.h"
#include "eigen3/Eigen/Eigen"
#include "eigen3/Eigen/Sparse"
using namespace Eigen;
//!This class represents the c_elegans system
/*!
 * This class calculates the voltages and currents in the neural network of a c_elegans worm.
 * In addition, it also allows for the ablation of specific neurons within the system
 * to see how the system responds to the removal of neurons.
 */
class c_elegans:public rhs_type<double, rhs_sde>{
    int cur_ind;
    double dummy;
    constexpr static size_t num_neur = 279;
    constexpr static size_t al_num = 4;
    typedef Eigen::SparseMatrix<double, RowMajor> sparse_type;
    sparse_type laplacian, AEchem_trans, AEchem_trans_full, ag_full;
    Array<double, num_neur, 1> vmean, sig, Echem, eqS, eqV;
    constexpr static size_t dim_v = num_neur;
    constexpr static size_t dim_s = dim_v;
    double beta, memV, memG, gchem, gelec, tau, EchemEx, EchemInh, ar, ad;
    std::vector<size_t> inj_nodes;
    std::vector<size_t> abl_neur;
    double cur_inj;
    bool has_gone;
    bool first_round;
    public:
    virtual void update();
    std::string type() const;
    void postprocess(input& in);
    std::vector<std::string> dependencies() const;
    int dxdt(ptr_passer x, ptr_passer dx, double dt);
    int dwdt(ptr_passer x, ptr_passer dx, double dt);
    void mod_w(ptr_passer w, double t);
    void initial_condition(ptr_passer in, size_t len);
};
#endif
