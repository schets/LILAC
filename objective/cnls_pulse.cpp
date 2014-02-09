#include "cnls_pulse.h"
#include "comp_funcs.h"


/*!
 * This is the objective function for objects that have a cnls-like form-
 * two coupled waves where the best solution is for each one to be a pulse.
 * It takes the energy of each pulse, and divides by the kurtosis of the
 * waveform of the laser pulse. This optimizes for energetic yet stable pulses
 */
double cnls_pulse::score(comp* restr invals){
    size_t dim = dimension/2;
    //could use energy function here, but it's simpler to just compute
    //it when calculating the helper array
    help[0] = sqrt(_sqabs(invals[0]) + _sqabs(invals[dim]));
    double ener = help[0];
    for(size_t i = 1; i < dim-1; i++){
        help[i] = _sqabs(invals[i]) + _sqabs(invals[dim+i]);
        ener += 2*help[i];
        help[i]=sqrt(help[i]);
    }
    help[dim-1]= _sqabs(invals[dim-1]) + _sqabs(invals[dimension-1]);
    ener += help[dim-1];
    help[dim-1]=sqrt(help[dim-1]);
    ener = std::sqrt(ener);
    double kurtosis_v=0;
    return kurtosis_v*ener;
}
