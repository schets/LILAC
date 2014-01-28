#ifndef COMP_FUNCS_H
#define COMP_FUNCS_H
#include "defs.h"
#include <cmath>
#include <vector>
#include <string>
extern "C"{
#include <stdlib.h>
#include <fftw3.h>
}
//THis file contains inlined complex functions
//The C and C++ standard treat complex as an array of two doubles
//but the C standard doesn't always have inlined complex functions
//and the C++ standard uses templates and wreaks havoc with
//the compiler
//So these operate directly on the double values in the array
//and provide nice inlined functions that should also
//work nicely with SIMD instructions
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
inline double _abs(comp inval){
    double r, i;
    r = ((double* restr)&inval)[0];
    i = ((double* restr)&inval)[1];
    return std::sqrt(r*r + i*i);
}
inline double _sqabs(comp inval){
    double r, i;
    r = ((double* restr)&inval)[0];
    i = ((double* restr)&inval)[1];
    return r*r + i*i;
}

inline double energy(comp* v, size_t s){
    double sum = _sqabs(v[0]);
    for(size_t i = 0; i < s; i++){
        sum += _sqabs(v[0]);
    }
    sum += _sqabs(v[s-2]);
    return sqrt(sum);
}

//This function returns a normalized fourier transform in either direction

inline void fft(fftw_plan pp, comp* restr in, comp* restr out, const size_t len){
    fftw_execute_dft(pp, in, out);
    
}
inline void ifft(fftw_plan pp, comp* restr in, comp* restr out, const size_t len){
    fftw_execute_dft(pp, in, out);
    double nval;
    nval=1.00/len;
    for(size_t i = 0; i < len; i++){
        out[i]*=nval;
    }
}
template<typename T> std::vector<T> appendvec(std::vector<T> a, std::vector<T> b){
    a.insert(a.begin(), b.begin(), b.end());
    return a;
}
#endif
