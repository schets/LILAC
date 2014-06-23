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
#include "n_pulse_score.h"
#include "comp_funcs.hpp"
#include "types/type_register.hpp"
//This calculates the kurtosis of an input array
template class type_register<n_pulse_score>;
static double kurtosis(double in[], size_t len){
    double mean, mom4, mom2;
    mean = mom4 = mom2 = 0;
    for(size_t i = 0; i < len; i++){
        mean += in[i];
    }
    mean /= len;
    for(size_t i = 0; i < len; i++){
        double sq = (in[i] - mean)*(in[i]-mean);
        mom2+= sq;
        mom4+= sq*sq;
    }
    mom4/=len;
    mom2/=len;
    return mom4/(mom2*mom2);
}
/*!
 * This is the objective function for objects that have a cnls-like form
 * n coupled waves where the best solution is for each one to be a pulse.
 * It takes the energy of each pulse, and divides by the kurtosis of the
 * waveform of the laser pulse. This optimizes for energetic yet stable pulses
 */
double n_pulse_score::score(comp* ucur){
    //This is not an optimal solution,
    //but isn't too slow and this code should never be a bottleneck
    //This verison is far easier to read/understand than the optimal version
    for(size_t i = 0; i < nts; i++){
        help[i] = _sqabs(ucur[i]);
        for(size_t j = 1; j < n_pulse; j++){
            help[i] += _sqabs(ucur[i+j*nts]);
        }
        help[i] = sqrt(help[i]);
        kurtosis_help[i] = help[i];
    }
    double ener = energy(help, nts);
    fft(kurtosis_help, kurtosis_help, nts);
    for(size_t i = 0; i < nts; i++){
        help[i] = abs(kurtosis_help[i]);
    }
    double kurtosis_v = 1.0/(kurtosis(help, nts));
    double score = kurtosis_v* ener;
    return score;
}
//!Returns the dependencies of n_pulse_score
/*!
 * This class has the same dependencies os the objective class,
 * along with:
 *
 *      - integer num_pulses: The number of pulses in the solution
 *
 */
std::vector<std::string> n_pulse_score::dependencies() const {
    std::string deps[] = {"num_pulses"};
    return make_append(deps, objective::dependencies());
}
void n_pulse_score::postprocess(input& invals){
    objective::postprocess(invals);
    int _n_pulse = 0;
    invals.retrieve(_n_pulse, "num_pulses", this);
    n_pulse = _n_pulse;
    if(dimension%n_pulse){
        err("n_pulse_score requires a dimension divisible by the number of pulses", 
                "bi_pulse_score::postprocess", "objective/bi_pulse_score.cpp", FATAL_ERROR);
    }
    nts = dimension/n_pulse;
    memp.create(nts, &help, &kurtosis_help);
}
n_pulse_score::~n_pulse_score(){}

std::string n_pulse_score::type() const{
    return "n_pulse_score";
}
