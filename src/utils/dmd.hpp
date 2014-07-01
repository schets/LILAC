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
#ifndef DMD_HPP
#define DMD_HPP
#include "defs.hpp"
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/SVD>
template<class T, class mat>
struct eigenval_type{
    typedef Eigen::EigenSolver<mat> solver;
};
template<class T, class mat>
struct eigenval_type<std::complex<T>, mat>{
    typedef Eigen::ComplexEigenSolver<mat> solver;
};
template<class T, int storage>
using dmat = Eigen::Matrix<T, Dynamic, Dynamic, storage>;
template<class T>
using dvec = Eigen::Matrix<T, Eigen::Dynamic, 1>;
//if invals is in R(n*m+1)
template<class T, int storage, int storage_out1, int storage_out2>
inline void _dmd_(const dmat<T, storage>& invals,
        dmat<T, storage_out1>& evecs, dvec<T>& evals, dmat<T, storage_out2>* pod_ptr,
        double energy){
        //from Tu, theory and application of DMD, we can use the thin models
        //(rank of input determines SVD dimensions
    Eigen::JacobiSVD<dmat<T, storage>>
        svd(invals.rightCols(invals.cols()-1),Eigen::ComputeThinU | Eigen::ComputeThinV);
    //trunctuate the svd after a certain energy point
    const auto& sing_vals = svd.singularValues();
    double ener =sing_vals.squaredNorm();
    decltype(svd.nonzeroSingularValues()) num_keep=0;
    double ener_cum_sum=0;
    while(ener_cum_sum < ener){
        ener_cum_sum += sing_vals[num_keep]*sing_vals[num_keep];
        num_keep++;
        if(num_keep >= svd.nonzeroSingularValues()){
            break;
        }
    }
    dmat<T, storage> S(num_keep, num_keep);
    //calculate the dmd
    S.noalias() = svd.matrixU().leftCols(num_keep).adjoint()*
        invals.leftCols(invals.cols()-1)*svd.matrixV().leftCols(num_keep) *
        (1.0/svd.singularValues().head(num_keep).array()).matrix().asDiagonal();
    //calculate eigenvalues/eigenvectors
    eigenval_type<T, dmat<T, storage>::solver esolver(S);
    if(esolver.info() != Eigen::Success){
        err("Eigenvalues failed to compute", "dmd",
                "utils/dmd.hpp", FATAL_ERROR);
    }
    evecs = esolver.eigenvectors();
    evals = esolver.eigenvalues();
    if(pod_ptr){
        *(pod_ptr) = svd.matrixU().leftCols(num_keep);
    }
}
template<class T, int storage, int storage_out1>
inline void dmd(const dmat<T, storage>& invals,
        dmat<T, storage_out1>& evecs, dvec<T>& evals, double energy){
   _dmd_(invals, evecs, evals, 0, energy);
}

template<class T, int storage, int storage_out1, int storage_out2>
inline void dmd(const dmat<T, storage>& invals,
        dmat<T, storage_out1>& evecs, dvec<T>& evals, dmat<T, storage_out2>& pod_dat,
        double energy){
    _dmd_(invals, evecs, evals, &pod_dat, energy);
}

template<class matrix>
inline matrix pod(const matrix& invals, double energy=1){
    Eigen::JacobiSVD<matrix>
        svd(invals.rightCols(invals.cols()-1),Eigen::ComputeThinU);
    //from Tu, theory and application of DMD, we can use the thin models
    //(rank of input determines SVD dimensions
    const auto& sing_vals = svd.singularValues();
    double ener =sing_vals.squaredNorm();
    decltype(sing_vals.nonzeroSingularValues()) num_keep=0;
    double ener_cum_sum=0;
    while(ener_cum_sum < ener){
        ener_cum_sum += sing_vals[num_keep]*sing_vals[num_keep];
        num_keep++;
        if(num_keep >= sing_vals.nonzeroSingularValues()){
            break;
        }
    }
    return svd.matrixU().leftCols(num_keep);
}
#endif
