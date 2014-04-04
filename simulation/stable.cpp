#include "stable.h"
#include "objective/objective.h"
#include "utils/comp_funcs.h"
#include "stable_ode_tmpl.hpp"
/*!
 * This function iterates the system forwards in time until it reaches a stable state
 * or a certain number of iterations, 
 * and then returns the objective function of that state.
 */
double stable::simulate(){
    bad_res=0;
    for (round = 0; round < max_iterations; round++){

        this->iterate_system();
        if(bad_res<0){
            return 0;
        }
        if(round){
            double chan = this->get_change();
            if(chan < change_threshold && round > num_min){
                break;
            }
        }

    }
    double v = this->score();
    if(1 || !num_gone || !(num_gone%1)){
        printf("System:%d, test# %d,  took %d iterations, score was %e\n",
                cont->index, num_gone, round, v);
    }

    num_gone++;
    return v;
}
std::vector<std::string> stable::dependencies() const{
    std::string deps[] = {"change_threshold", "max_iterations"};
    return appendvec(simulation::dependencies(), std::vector<std::string>(deps, deps+2));
}

void stable::postprocess(std::map<std::string, item*>& invals){
    num_gone=0;
    simulation::postprocess(invals);
    invals["change_threshold"]->retrieve(&change_threshold, this);
    if(change_threshold < 0){
        err("Input parameter change_threshold must be greater than or equal to zero",
                "stable::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
    invals["max_iterations"]->retrieve(&max_iterations, this);
    if(max_iterations <= 0){
        err("Input parameter max_iterations must be greater than or equal to zero",
                "stable::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
}
