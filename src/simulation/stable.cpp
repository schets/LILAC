/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
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
/*!
 * This function returns the dependencies of the stable class.
 * The stable class depends on:
 *
 *      - double change_threshold: When the change between two iterations is less than change_threshold, the iteration stops.
 *      - integer max_iterations: The number at which the iteration stops and the system is deemed unstable
 */
std::vector<std::string> stable::dependencies() const{
    std::string deps[] = {"change_threshold", "max_iterations"};
    return make_append(deps, simulation::dependencies());
}

void stable::postprocess(input& invals){
    num_gone=0;
    simulation::postprocess(invals);
    invals.retrieve(change_threshold, "change_threshold", this);
    if(change_threshold < 0){
        err("Input parameter change_threshold must be greater than or equal to zero",
                "stable::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
    invals.retrieve(max_iterations, "max_iterations", this);
    if(max_iterations <= 0){
        err("Input parameter max_iterations must be greater than or equal to zero",
                "stable::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
}
