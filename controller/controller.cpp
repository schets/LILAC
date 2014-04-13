#include "controller.h"
#include "comp_funcs.hpp"

#include "item_heads.hpp"
#include "toroidal.h"
//!Deprecated
void controller::parse(std::string inval){
}

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


/*!
 * This does the postprocessing for the base class postprocess
 * The postprocessing expects the names to be in the format
 * name1,name2,name3,name4
 * just for now, to have a working toroidal search, the
 * name list is hardcoded to be "a1, a2, a3, ap"
 * Engine will be more revamped fri+weekend
 * @param inval A map containing the input data
 * \sa item_dim::postprocess
 */
void controller::postprocess(input& inval){
    item_dim::postprocess(inval);
    std::string names;
    num_cont = 0;
    retrieve(names, inval["names"], this);
    retrieve(index, inval["!start_ind"], this);
    //perform processing on the names
    //also with typechecking
    //will do this right sometime
 /*   vars.resize(4);
    vars[0] = (variable*)inval["a1"];
    vars[1] = (variable*)inval["a2"];
    vars[2] = (variable*)inval["a3"];
    vars[3] = (variable*)inval["ap"];*/
}

item* controller::create(std::string name){
    if(name == "toroidal"){
        return new toroidal();
    }
    return 0;
}
