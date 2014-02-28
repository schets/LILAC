#ifndef COMP_FUNCS_H
#define COMP_FUNCS_H
#include "defs.h"
#include <cmath>
#include <vector>
#include <string>
extern "C"{
#include <fftw3.h>
}
//These are over twice as fast as using the standard library functions,
//or even using intrinsics. They allow vectorizing

inline comp cmul(comp _in1, comp _in2){
    double* restr in1 = (double* restr)&_in1;
    double* restr in2 = (double* restr)&_in2;
    double out[2] = {in1[0]*in2[0] - in1[1]*in2[1], in1[0]*in2[1]+in1[1]*in2[0]};
    return *(comp*)out;
}
inline comp cmul(comp _in1, double in2){
    double* restr in1 = (double* restr)&_in1;
    double out[2] = {in1[0]*in2, in1[1]*in2};
    return *(comp*)out;
}
inline comp cmul(double in1, comp _in2){
    double* restr in2 = (double* restr)&_in2;
    double out[2] = {in1*in2[0], in1*in2[1]};
    return *(comp*)out;
}
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
inline void trim(std::string& curline){
    size_t startpos = curline.find_first_not_of("\n\r\t ");
    if(startpos != std::string::npos){
        curline=curline.substr(startpos);
    }
    startpos = curline.find_last_not_of("\n\r\t ");
    if(startpos != std::string::npos){
        curline=curline.substr(0, startpos+1);
    }
}
inline void trim(std::string& curline, char val){
    size_t startpos = curline.find_first_not_of(val);
    if(startpos != std::string::npos){
        curline=curline.substr(startpos);
    }
    startpos = curline.find_last_not_of(val);
    if(startpos != std::string::npos){
        curline=curline.substr(0, startpos+1);
    }
}
inline void ltoken(std::string& tok, std::string& str, std::string delim=" "){
    size_t tpos = str.find(delim);
    tok=str.substr(0, tpos);
    str.erase(0, tpos + delim.length());
}

#endif
