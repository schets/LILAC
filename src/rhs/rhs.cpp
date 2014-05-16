#include "rhs.h"
/*!
 * Provides the default postprocessing for the rhs base class
 * which simply calls the postprocessing for item_dim
 * @param dat The map containing the io data, may be modified by function
 * \sa item_dim::postprocess
 */
void rhs::postprocess(input& dat){
    item_dim::postprocess(dat);
}

/*!
 * Returns a vector of dependencies
 * rhs has no explicit dependencies except for those of the parent class
 * @return A vector containing the names of the dependencies
 * \sa item_dep::dependencies
 * */
std::vector<std::string> rhs::dependencies()const {
    return item_dim::dependencies();
}

