#ifndef NOISE_HPP
#define NOISE_HPP
#include "defs.hpp"
#include "float_traits.hpp"
double get_norm_rand( double sigma);
template<class T>
inline void gaussian_noise(T* restr inval, size_t len, double sdev){
    typedef typename float_traits<T>::type real_type;
    for(size_t i = 0; i < len; i++){
	auto retval = float_traits<T>::get_rep(inval[i]);
	for(size_t j = 0; j < retval.size; j++){
		retval.vals[j]=(real_type)get_norm_rand(sdev);
	}
    }
}
#endif
