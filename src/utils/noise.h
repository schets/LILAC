#ifndef NOISE_HPP
#define NOISE_HPP
#include "defs.hpp"
#include "types/type_operators.hpp"
double get_norm_rand( double sigma);
template<class T>
inline void gaussian_noise(T* inval, size_t len, double sdev){
    typedef typename float_traits<T>::type real_type;
    for(size_t i = 0; i < len; i++){
        apply<T>::transform(inval[i], [sdev](real_type& val, size_t){
                    val = get_norm_rand(sdev);
                });
    }
}
#endif
