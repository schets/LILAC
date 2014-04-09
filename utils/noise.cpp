#include "noise.h"
#include "comp_funcs.hpp"
#include <random>
void fill_gaussian_noise(double* restr inv, size_t len, double sdev){
    ALIGNED(inv);
    static std::ranlux48_base gener;
    std::normal_distribution<double> ngen(0.0, 1.0);
    for(size_t i = 0; i < len; i++){
        inv[i] = std::abs(ngen(gener));
    }
}
void gaussian_noise_double(comp* restr inval, size_t len, double sdev){
    fill_gaussian_noise((double* restr)inval, 2*len, sdev);
  /*  ALIGNED(inval);
    double nval;
    for(size_t i = 0; i < len; i++){
        nval += _sqabs(inval[i]);
    }
    nval = std::sqrt(nval);
    for(size_t i = 0; i < len; i++){
        inval[i] *= norm / nval;
    }*/
}
