#include "engineimp.h"
#include "utils/comp_funcs.h"
#include "simulation/simulation.h"
void engineimp::run(){
    simulation* sys;
    values["simulation"]->retrieve(&sys, 0);
    controller* cont;
    values["controller"]->retrieve(&cont, 0);
    cont->pre_set();
    update();
    while(cont->is_good()){
        sys->simulate();
        cont->control(0, 0);
        update();
    }
    fft_cleanup();
}
