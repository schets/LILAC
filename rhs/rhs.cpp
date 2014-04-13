#include "rhs.h"
#include "rhs_CNLS.h"
#include "rhs_SQGLE.h"
#include "example_rhs.h"
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
 * Creates an rhs object
 * @param tname The name of the type to be returned
 * @return A pointer to the rhs object
 * \sa item::create
 * */
rhs* rhs::create(std::string tname){
    if(tname == "CNLS"){
        return new rhs_CNLS();
    }
    else if(tname == "SQGLE"){
        return new rhs_SQGLE();
    }
    else if(tname=="example_rhs"){
        err("Example_rhs created, mysteriously fails on some architectures seemingly from AVX instructions. Also does nothing useful", "rhs::create", "rhs/rhs.cpp", WARNING);
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
