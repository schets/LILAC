#include "objective.h"
#include "n_pulse_score.h"
//#include <gsl/gsl_stats_double.h>

/*!
 * This function returns the dependencies of the objective class
 * which are only the dependencies of item_dim
 * @return Dependencies of item_dim
 * \sa item_dim::dependencies
 */
std::vector<std::string> objective::dependencies() const{
    return item_dim::dependencies();
}

/*!
 * This does the postprocessing for objective, which simply calls item_dim::postprocess
 * @param invals The map containing the postprocessed values
 * \sa item_dim::postprocessing
 */
void objective::postprocess(input& invals){
    item_dim::postprocess(invals);
} 

item* objective::create(std::string name){
    if(name=="n_pulse_score"){
        return new n_pulse_score();
    }
    return 0;
}
