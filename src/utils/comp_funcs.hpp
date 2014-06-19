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
#ifndef COMP_FUNCS_H
#define COMP_FUNCS_H
#include "defs.hpp"

//Although I usually want to remove redundnant headers, this header is going to be included by
//99% of the files that include comp_funcs.hpp"
//If that stops being true in the future, I'll change it
#include "item_heads.hpp"
//These are over twice as fast as using the standard library functions,
//or even using intrinsics. They allow vectorizing
using std::abs;
inline comp _conj(comp inval){
    ((double* restr)&inval)[1] *= -1;
    return inval;
}
inline double _real(comp inval){
    return ((double* restr)&inval)[0];
}
inline double _imag(comp inval){
    return ((double* restr)&inval)[1];
}
inline double _sqabs(comp inval){
    double r, i;
    r = ((double* restr)&inval)[0];
    i = ((double* restr)&inval)[1];
    return r*r + i*i;
}
inline double energy(comp* v, size_t s){
    double sum = _sqabs(v[0]);
    for(size_t i = 1; i < s-1; i++){
        sum += 2*_sqabs(v[i]);
    }
    sum += _sqabs(v[s-1]);
    return sqrt(sum/2);
}
inline double energy(double* v, size_t s){
    double sum = v[0]*v[0];
    for(size_t i = 1; i < s-1; i++){
        sum += 2*v[i]*v[i];
    }
    sum += v[s-1]*v[s-1];
    return sqrt(sum/2);
}
template<class T>
T trap(T* restr v, size_t s){
    ALIGNED(v);
    T sum = T();
    for(size_t i=1; i < s-1; i++){
        sum += v[i];
    }
    sum += (v[0] + v[s-1])/2.0;
    return sum;
}

//These fourier transforms have to do a dirty pointer transform from std::complex
//to fftw_complex. Since std::complex<double> is bitwise the same as the C complex, and fftw_complex is
//the same as the C complex, this works. simply using fftw_complex works in c++03, but c++11 has some
//very nice template features for dealing with runtime type construction and inheritance validation
//

//!This function performs a fourier transform in the forward direction
void fft(comp* _in, comp* _out, const size_t len);
//!This functions performs a normalized fourier transform in the backwards direction
void ifft(comp* _in, comp* _out, const size_t len);
void fft_cleanup();
template<typename T>
inline std::vector<T> appendvec(std::vector<T> a, const std::vector<T>& b){
    a.insert(a.begin(), b.begin(), b.end());
    return a;
}
template<class T, size_t N>
inline std::vector<T> makevec(const T(& array)[N]){
    return std::vector<T>(array, array+N);
}
template<class T, size_t N>
inline std::vector<T> make_append(const T (&array)[N], const std::vector<T>& b){
    return appendvec(makevec(array), b);
}

#endif
