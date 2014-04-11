#pragma once
#ifndef COMP_FUNCS_H
#define COMP_FUNCS_H
#include "defs.h"
#include <cmath>
#include <string>
#include <vector>
extern "C"{
#include <fftw3.h>
}
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

//These fourier transforms have to do a dirty pointer transform from std::complex
//to fftw_complex. Since std::complex<double> is bitwise the same as the C complex, and fftw_complex is
//the same as the C complex, this works. simply using fftw_complex works in c++03, but c++11 has some
//very nice template features for dealing with runtime type construction and inheritance validation
//

//!This function performs a fourier transform in the forward direction
void fft(comp* _in, comp* _out, const size_t len);
//!This functions performs a normalized fourier transform in the backwards direction
void ifft(comp* _in, comp* _out, const size_t len);

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
void trim(std::string& curline);
void trim(std::string& curline, char val);
void ltoken(std::string& tok, std::string& str, std::string delim=" ");
std::string get_unique_name(std::string base);
#endif
