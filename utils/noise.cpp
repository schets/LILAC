#include "noise.h"
#include "comp_funcs.hpp"
#include <chrono>
#include <random>
void fill_gaussian_noise(double* restr inv, size_t len, double sdev){
    ALIGNED(inv);
    static std::ranlux48_base gener(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::normal_distribution<double> ngen(0.0, sdev);
    for(size_t i = 0; i < len; i++){
        inv[i] = ngen(gener);
    }
}
void fill_gaussian_noise(float* restr inv, size_t len, double sdev){
    ALIGNED(inv);
    static std::ranlux48_base gener(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::normal_distribution<float> ngen(0.0, sdev);
    for(size_t i = 0; i < len; i++){
        inv[i] = ngen(gener);
    }
}
