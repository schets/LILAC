#include "objective.h"

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
