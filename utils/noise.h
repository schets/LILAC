#ifndef NOISE_HPP
#define NOISE_HPP
#include "defs.hpp"
void fill_gaussian_noise(double* restr inv, size_t len, double sdev);
template<class T>
void gaussian_noise_double(T* restr inval, size_t len, double sdev){
    static_assert(sizeof(T) >= sizeof(double), "Type is too small to be used in gaussian noise double");
    fill_gaussian_noise((double*)inval, (sizeof(T)/sizeof(double))*len, sdev);
    ALIGNED(inval);
/*    double nval;
    for(size_t i = 0; i < len; i++){
        double aval = std::abs(inval[i]);
        nval += aval*aval;
    }
    nval = std::sqrt(nval);
    for(size_t i = 0; i < len; i++){
        inval[i] *= norm / nval;
    }*/
}
void gaussian_noise_double(comp* restr inval, size_t len, double norm);
#endif
