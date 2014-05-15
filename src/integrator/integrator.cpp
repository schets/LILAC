#include "integrator.h"
#include "rk4.h"
#include "rk45.h"
#include "euler_sde.h"
#include "comp_funcs.hpp"
/*!
 * This function returns the dependencies of the integrator class
 * \note
 * Integrator has no explicit dependencies, returns item_dim::dependencies
 *
 * @return A vector containing the dependencies for integrator
 * \sa item_dim::dependencies
 */
std::vector<std::string> integrator::dependencies() const {
    std::string rhsval[] = {"rhs"};
    return make_append(rhsval, item_dim::dependencies());
}

//! Initiates the integrator from input values
/*!
 * This function initializes the internal memory and variables in the initiator class
 * When called, all parameters returned by dependencies are promised to have been processed
 * at the appropriate level of detail
 * \sa item_dim::postprocess
 */
void integrator::postprocess(input& dat){
    item_dim::postprocess(dat); 
    retrieve(rh_val, dat["rhs"], this);
}

void integrator::initial_condition(ptr_passer in, size_t len){
    rh_val->initial_condition(in, len);
}
