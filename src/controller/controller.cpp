#include "controller.h"
#include "comp_funcs.hpp"
#include "item_heads.hpp"
/*!
 * This function returns the dependencies of the controller class
 * \note This class depends on the variable names, as well as the dependencies for
 * item_dim
 * @return The list of dependencies for the controller class, which is "names"
 * \sa item_dim::dependencies
 */
std::vector<std::string> controller::dependencies() const{
    std::string deps[] = {"names"};
    return make_append(deps, item_dim::dependencies());
}

void controller::postprocess(input& inval){
    item_dim::postprocess(inval);
    num_cont = 0;
    inval.retrieve(index, "!start_ind", this);
 }
