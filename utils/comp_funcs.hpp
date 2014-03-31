#pragma once
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
inline double abs(const comp inval){
        double r, i;
        r = ((double* restr)&inval)[0];
        i = ((double* restr)&inval)[1];
        return std::sqrt(r*r + i*i);
    }
inline double abs(const double inval){
    return std::abs(inval);
}
inline float abs(const float inval){
    return std::abs(inval);
}
/*inline comp cmul(comp _in1, comp _in2){
    double* restr in1 = (double* restr)&_in1;
    double* restr in2 = (double* restr)&_in2;
    double out[2] = {in1[0]*in2[0] - in1[1]*in2[1], in1[0]*in2[1]+in1[1]*in2[0]};
    return *(comp*)out;
}*/
/*
inline comp cmul(comp _in1, double in2){
    double* restr in1 = (double* restr)&_in1;
    double out[2] = {in1[0]*in2, in1[1]*in2};
    return *(comp*)out;
}
inline comp cmul(double in1, comp _in2){
    double* restr in2 = (double* restr)&_in2;
    double out[2] = {in1*in2[0], in1*in2[1]};
    return *(comp*)out;
}*/
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
//These fourier transforms have to do a dirty pointer transform from std::complex
//to fftw_complex. Since std::complex<double> is bitwise the same as the C complex, and fftw_complex is
//the same as the C complex, this works. simply using fftw_complex works in c++03, but c++11 has some
//very nice template features for dealing with runtime type construction and inheritance validation
//

//!This function returns a fourier transform in the forward direction

inline void fft(comp* _in, comp* _out, const size_t len){
    fftw_complex* in = (fftw_complex*)_in;
    fftw_complex* out = (fftw_complex*)_out;
    static std::map<size_t, fftw_plan> in_place, out_place;
    if(in == out){
        if(!in_place.count(len)){
            in_place[len] = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        }
        fftw_execute_dft(in_place[len], in, out);
        return;
    }
    if(!out_place.count(len)){
        out_place[len] = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    }
    fftw_execute_dft(out_place[len], in, out);
}
//!Wrapper function for compatibility
inline void fft(fftw_plan pp, comp* in, comp* out, const size_t len){
    fft(in, out, len);
}
inline void ifft(comp* _in, comp* _out, const size_t len){
    fftw_complex*  in = (fftw_complex*)_in;
    fftw_complex*  out = (fftw_complex*)_out;
    static std::map<size_t, fftw_plan> in_place, out_place;
    if(in == out){
        if(!in_place.count(len)){
            in_place[len] = fftw_plan_dft_1d(len, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
        }
        fftw_execute_dft(in_place[len], in, out);
    }
    else{
        if(!out_place.count(len)){
            out_place[len] = fftw_plan_dft_1d(len, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
        }
        fftw_execute_dft(out_place[len], in, out);
    }
    double nval;
    nval=1.00/len;
    for(size_t i = 0; i < len; i++){
        _out[i]*=nval;
    }
}
inline void ifft(fftw_plan pp, comp* restr in, comp* restr out, const size_t len){
    ifft(in, out, len);
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
//!Hides the contents from the rest of the program
namespace __HIDER__{
    //!Create a unique name that cannot already exist in the engine, should never reference directly
    class _unique_name{
        std::set<std::string> names;
        public:
        std::string get_unique_name(std::string nbase){
            nbase.push_back('_');
            while(names.count(nbase)){
                nbase.push_back(1 | (char)(rand()));
            }
            names.insert(nbase);
            //ensure that the name will not be alread in the engine
            nbase.push_back('!');
            return nbase;
        }
        _unique_name(){
            srand(time(0));
        }
    };
}
/*!
 * Generates a unique name for an object that is promised never to exist in the engine
 * as long as all extra names are generated using this function
 *
 * @param base The base string with which to generate the name
 * @return A unique name based on the input string
 */
/*inline std::string get_unique_name(std::string base){
  static __HIDER__::_unique_name nn;
  return nn.get_unique_name(base);
  }*/
#endif
