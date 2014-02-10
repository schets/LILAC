#include "bi_pulse_score.h"
#include "comp_funcs.h"
extern "C"{
#include <gsl/gsl_statistics_double.h>
}
double mom4(comp*, double*, int);

/*!
 * This is the objective function for objects that have a cnls-like form-
 * two coupled waves where the best solution is for each one to be a pulse.
 * It takes the energy of each pulse, and divides by the kurtosis of the
 * waveform of the laser pulse. This optimizes for energetic yet stable pulses
 */
double bi_pulse_score::score(comp* ucur){
        double ener=0;
    help[0] = sqrt(_sqabs(ucur[0]) + _sqabs(ucur[nts]));
    ener = help[0];
    kurtosis_help[0] = help[0];
    for(size_t i = 1; i < nts-1; i++){
        help[i] = _sqabs(ucur[i]) + _sqabs(ucur[nts+i]);
        ener += 2*help[i];
        help[i]=sqrt(help[i]);
        kurtosis_help[i] = help[i];
    }
    help[nts-1]= _sqabs(ucur[nts-1]) + _sqabs(ucur[2*nts-1]);
    ener += help[nts-1];
    help[nts-1]=sqrt(help[nts-1]);
    kurtosis_help[nts-1]=help[nts-1];
    ener = sqrt(ener);
    fft(ffor, kurtosis_help, kurtosis_help, nts);
    for(int i = 0; i < nts; i++){
        help[i] = _abs(kurtosis_help[i]);
    }
    double kurtosis_v = 1.0/(gsl_stats_kurtosis(help, 1, nts));
    double score = kurtosis_v* ener;
    return score;
}
std::vector<std::string> bi_pulse_score::dependencies() const {
    return objective::dependencies();
}
void bi_pulse_score::postprocess(std::map<std::string, item*>& invals){
    objective::postprocess(invals);
    if(dimension%2){
        err("bi_pulse_score requires an even dimension", "bi_pulse_score::postprocess",
                "objective/bi_pulse_score.cpp", FATAL_ERROR);
    }
    nts = dimension/2;
    help = (double*)al_malloc(nts*sizeof(double));
    kurtosis_help = (comp*)al_malloc(nts*sizeof(comp));
    ffor = fftw_plan_dft_1d(nts, kurtosis_help, kurtosis_help, FFTW_FORWARD, FFTW_ESTIMATE);
}
bi_pulse_score::~bi_pulse_score(){
    al_free(help);
    al_free(kurtosis_help);
    fftw_destroy_plan(ffor);
}
std::string bi_pulse_score::type() const{
    return "bi_pulse_score";
}
