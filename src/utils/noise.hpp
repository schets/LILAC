/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#ifndef NOISE_HPP
#define NOISE_HPP
#include "defs.hpp"
#include "types/type_operators.hpp"
#include <stdlib.h>
#include <time.h>
#include <acml.h>
namespace __HIDER__{
    //!rng generator struct;
    struct acml_rng{
        constexpr static int rand_len=64;
        constexpr static int genid=3;
        double vals[rand_len];
        double* const val_end;
        double* cur_val;
        int subid=1;
        int id;
        constexpr static int len_state=633;
        int state[len_state];
        int lstate=len_state;
        int info;
        acml_rng():val_end((&(vals[0]))+rand_len){
            int seed = (int)clock();
            while(seed <= 1){
                seed = (int)clock();
            }
            int lseed = 1;
            drandinitialize(genid, subid, &seed, &lseed, state, &lstate, &info);
            if(info == 1){
                drandinitialize(genid, subid, &seed, &lseed, state, &lstate, &info);
            }
            drandgaussian(rand_len, 0, 1, state, vals, &info);
        }
        void fill(double* in, size_t len, double mean, double var){
            drandgaussian(len, mean, var, state, in, &info);
        }
        double get_gauss_double(double mean, double sdev){
            if(PREDICT(cur_val < val_end, 1)){
                return mean + (*cur_val++)*sdev;
            }
            cur_val = (&(vals[0]))+1;
            drandgaussian(rand_len, 0, 1, state, vals, &info);
            return mean + vals[0]*sdev;
        }
    };
}
//!This functions fills the input data with random gaussian noise
/*!
 * This functions fills each variable of the 
 */
template<class T, class Lambda>
inline void gaussian_noise(T* inval, size_t len, double mean, double sdev, Lambda&& fnc){
    //!rng generator struct;
    static __HIDER__::acml_rng rng;
    auto* rng_ptr = &rng; //This bypasses a compiler warning
    typedef typename float_traits<T>::type real_type;
    if(float_traits<T>::cast_to_double){
        rng.fill((double*)inval, len*float_traits<T>::dim, mean, sdev*sdev);
        for(size_t i = 0; i < len; i++){
            std::forward<Lambda>(fnc)(inval[i]);
        }
    }
    else{
        for(size_t i = 0; i < len; i++){
            apply<T>::transform(inval[i],
                    [sdev, mean, rng_ptr](real_type& val, size_t){
                    val = real_type(rng_ptr->get_gauss_double(mean, sdev));
                    },
                    std::forward<Lambda>(fnc));
        }
    }
}
template<class T>
inline void gaussian_noise(T* inval, size_t len, double mean, double sdev){
    gaussian_noise(inval, len, mean, sdev, [](T&){});
}
#endif
