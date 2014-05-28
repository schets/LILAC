#include "stable.h"
#include "objective/objective.h"
#include "utils/comp_funcs.hpp"
#include "stable_ode_tmpl.hpp"
#include "writer/writer.h"
#include "engine/engineimp.h"
const static int num_min = 10;
/*!
 * This function iterates the system forwards in time until it reaches a stable state
 * or a certain number of iterations, 
 * and then returns the objective function of that state.
 */
double stable::simulate(){
    bad_res=0;
    cur_writer = std::shared_ptr<writer>(new writer(true));
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
    bool is_stable = (round <= 45);
    cur_writer->add_data(data::create("is_stable", is_stable), writer::OTHER);
    double v = this->score();
    cur_writer->add_data(data::create("score", v), writer::FINAL_SCORE);
    comp* uc = ((stable_ode_tmpl<comp>*)this)->ucur;
    cur_writer->add_data(
            data::create("final function", uc, dimension),
                writer::FINAL_FUNC);
    if(!num_gone || !(num_gone%10)){
        printf("System:%d, test# %d,  took %d iterations, score was %e\n",
                cont->index, (int)cont->num_cont, round, v);
    }
    holder->add_writer(cur_writer);
    num_gone++;
    return v;
}
std::vector<std::string> stable::dependencies() const{
    std::string deps[] = {"change_threshold", "max_iterations"};
    return make_append(deps, simulation::dependencies());
}

void stable::postprocess(input& invals){
    num_gone=0;
    simulation::postprocess(invals);
    retrieve(change_threshold, invals["change_threshold"], this);
    if(change_threshold < 0){
        err("Input parameter change_threshold must be greater than or equal to zero",
                "stable::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
    retrieve(max_iterations, invals["max_iterations"], this);
    if(max_iterations <= 0){
        err("Input parameter max_iterations must be greater than or equal to zero",
                "stable::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
}
