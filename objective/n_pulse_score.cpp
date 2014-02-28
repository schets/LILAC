#include "n_pulse_score.h"
#include "comp_funcs.h"
double kurtosis(double in[], size_t len){
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
    fft(ffor, kurtosis_help, kurtosis_help, nts);
    for(size_t i = 0; i < nts; i++){
        help[i] = _abs(kurtosis_help[i]);
    }
    double kurtosis_v = 1.0/(kurtosis(help, nts));
    double score = kurtosis_v* ener;
    return score;
}
std::vector<std::string> n_pulse_score::dependencies() const {
    std::string deps[] = {"num_pulses"};
    return appendvec(objective::dependencies(), std::vector<std::string>(deps, deps+1));
}
void n_pulse_score::postprocess(std::map<std::string, item*>& invals){
    objective::postprocess(invals);
    invals["num_pulses"]->retrieve(&n_pulse, this);
    if(dimension%n_pulse){
        err("n_pulse_score requires a dimension divisible by the number of pulses", 
                "bi_pulse_score::postprocess", "objective/bi_pulse_score.cpp", FATAL_ERROR);
    }
    nts = dimension/n_pulse;
    help = (double*)al_malloc(nts*sizeof(double));
    kurtosis_help = (comp*)al_malloc(nts*sizeof(comp));
    ffor = fftw_plan_dft_1d(nts, kurtosis_help, kurtosis_help, FFTW_FORWARD, FFTW_ESTIMATE);
}
n_pulse_score::~n_pulse_score(){
    al_free(help);
    al_free(kurtosis_help);
    fftw_destroy_plan(ffor);
}
std::string n_pulse_score::type() const{
    return "n_pulse_score";
}
