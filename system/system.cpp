#include "system.h"
#include "comp_funcs.h"
void system::checkpoint(){
}
void system::postprocess(std::map<std::string, item*>& inval){
}

/*!
 * This returns the dependencies for the system class, which are
 * "controller" and the dependencies for item_dim
 * \note
 * System depends on "controller"
 * \sa item_dim::dependencies
 * @return Dependencies for system
 */
std::vector<std::string> system::dependencies() const{
    std::string sys_deps[]={"controller"};
    return appendvec(std::vector<std::string>(sys_deps, sys_deps+1), item_dim::dependencies());
}
