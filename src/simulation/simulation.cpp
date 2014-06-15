#include "simulation.h"
#include "utils/comp_funcs.hpp"
void simulation::postprocess(input& invals){
    item_dim::postprocess(invals);
    invals.retrieve(cont, "controller", this);
    invals.retrieve(obj, "objective", this);
    invals.retrieve(out_f_name, "!out_file", this);
}

/*!
 * This returns the dependencies for the simulation class, which are
 * "controller" and the dependencies for item_dim
 * \note
 * System depends on "controller"
 * \sa item_dim::dependencies
 * @return Dependencies for system
 */
std::vector<std::string> simulation::dependencies() const{
    std::string sys_deps[]={"controller", "objective"};
    return make_append(sys_deps, item_dim::dependencies());
}

double simulation::score(){
    return 0;
}
