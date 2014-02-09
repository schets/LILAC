#include "simulation.h"
#include "stable.h"
#include "jones_optical.h"
#include "comp_funcs.h"
void simulation::postprocess(std::map<std::string, item*>& invals){
    item_dim::postprocess(invals);
    invals["controller"]->retrieve(&cont, this);
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
    std::string sys_deps[]={"controller"};
    return appendvec(std::vector<std::string>(sys_deps, sys_deps+1), item_dim::dependencies());
}

item* simulation::create(std::string name){
    if(name == "jones_optical"){
        return new jones_optical();
    }
   /* else if(name == "stable_ode"){
        return new stable_ode();
    }*/
    return 0;
}
