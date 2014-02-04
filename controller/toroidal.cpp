#include "parser/item.h"
#include "toroidal.h"
#include "comp_funcs.h"
/*
 * This function returns the dependencies of the toroidal class
 * \note
 * This class depends on the parameter iterations, which is the number of iterations that
 * the search will perform
 *
 * @return returns the dependencies of the toroidal class,
 * along with the dependencies of the controller class
 *\sa controller::dependencies, item_dim::dependencies
 */
std::vector<std::string> toroidal::dependencies() const{
    std::string deps[] = {"iterations"};
    return appendvec(std::vector<std::string>(deps, deps+1), controller::dependencies());
}


/*!
 * Performs the postprocessing for toroidal,
 * which is setting the number of iterations that are to be performed
 * @param dat Map containing the input values
 * \sa controller::postprocess, item_dim::postprocess
 */
void toroidal::postprocess(std::map<std::string, item*>& dat){
    controller::postprocess(dat);
    dat["iterations"]->retrieve(&iterations, this);
}

/*!
 * Returns the type of the class toroidal
 * @return Returns the type of the toroidal class, "toroidal"
 */
std::string toroidal::type() const{
    return "toroidal";
}

void toroidal::control(comp* u, objective* obj){
    for(int i = 0; i < vars.size(); i++){
        vars[i]->inc();
    }
}
