#include "engineimp.h"
#include "utils/comp_funcs.hpp"
#include "simulation/simulation.h"
#include "item_heads.hpp"
void engineimp::run(){
    auto sys = std::dynamic_pointer_cast<simulation>(values["simulation"].get_shared());
    auto cont = std::dynamic_pointer_cast<controller>(values["controller"].get_shared());
    if(!((bool)sys && (bool)cont)){
        err("The engine requires a simulation class called sys and a controller class called cont",
                "engineimp::engine_run", "engine/engine_run.cpp", FATAL_ERROR);
    }
    cont->pre_set();
    update();
    index=0;
    while(cont->is_good()){
        sys->simulate();
        index++;
        cont->control(0, 0);
        update();
    }
    fft_cleanup();
}
