#include "integrator.h"
#include "rk4.h"
#include "rk45.h"
#include "comp_funcs.h"
/*!
 * This function returns the dependencies of the integrator class
 * \note
 * Integrator has no explicit dependencies, returns item_dim::dependencies
 *
 * @return A vector containing the dependencies for integrator
 * \sa item_dim::dependencies
 */
std::vector<std::string> integrator::dependencies() const {
    std::string rhsval = "rhs";
    std::vector<std::string> temp= item_dim::dependencies();
    temp.push_back(rhsval);
    return temp;
}

//! Initiates the integrator from input values
/*!
 * This function initializes the internal memory and variables in the initiator class
 * When called, all parameters returned by dependencies are promised to have been processed
 * at the appropriate level of detail
 * \sa item_dim::postprocess
 */
void integrator::postprocess(std::map<std::string, std::shared_ptr<item> >& dat){
    item_dim::postprocess(dat); 
    dat["rhs"]->retrieve(&rh_val, this);
}
//!Returns an integrator* corresponding to the string passed
/*!
 * This function returns an integrator that is of type inval,
 * the string passed to the function
 * @param inval A string that is the name of the integrator type to be created
 * @return The integrator type corresponding to inval
 * \sa item::create
 */
item* integrator::create(std::string inval){
    if(inval=="rk4"){
        return new rk4();
    }
    if(inval=="rk45"){
        return new rk45();
    }
    return 0;
}
