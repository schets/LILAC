#ifndef NOISE_HPP
#define NOISE_HPP
#include "defs.hpp"
void fill_gaussian_noise(double* restr inv, size_t len, double sdev);
void fill_gaussian_noise(float* restr inv, size_t len, double sdev);
template<class T>
inline void gaussian_noise(T* restr inval, size_t len, double sdev){
    static_assert(sizeof(T) >= sizeof(double), "Type is too small to be used in gaussian noise double");
    static_assert(!(sizeof(T) % sizeof(double)),
            "Type is not a multiple size of the double variable");
    fill_gaussian_noise((double*)inval, (len * sizeof(T))/sizeof(double), sdev);
}
template<>
inline void gaussian_noise(float* inval, size_t len, double sdev){
    fill_gaussian_noise(inval, len, sdev);
}
#endif
