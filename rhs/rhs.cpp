#include "rhs.h"
#include "rhs_imp.h"
#include "example_rhs.h"
#include <cstring>
/*!
 * Provides the default postprocessing for the rhs base class
 * which simply calls the postprocessing for item_dim
 * @param dat The map containing the io data, may be modified by function
 * \sa item_dim::postprocess
 */
void rhs::postprocess(std::map<std::string, item*>& dat){
    item_dim::postprocess(dat);
}
/*!
 * Creates an rhs object
 * @param tname The name of the type to be returned
 * @return A pointer to the rhs object
 * \sa item::create
 * */
rhs* rhs::create(std::string tname){
    if(tname == "CNLS"){
        return new rhs_CNLS();
    }
    else if(tname=="example_rhs"){
        return new example_rhs();
    }
    
    return 0;
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
