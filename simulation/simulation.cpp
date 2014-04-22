#include "stable.h"
#include "jones_optical.h"
#include "utils/comp_funcs.hpp"

void simulation::postprocess(input& invals){
    item_dim::postprocess(invals);
    retrieve(cont, invals["controller"], this);
    retrieve(obj, invals["objective"], this);
    retrieve(out_f_name, invals["!out_file"], this);
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

item* simulation::create(std::string name){
    if(name == "jones_optical"){
        return new jones_optical();
    }
    return 0;
}
double simulation::score(){
    return 0;
}
