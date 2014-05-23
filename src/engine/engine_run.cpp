#include "engineimp.h"
#include "utils/comp_funcs.hpp"
#include "simulation/simulation.h"
#include "item_heads.hpp"
void engineimp::run(){
    simulation* sys=0;
    retrieve(sys, values["simulation"], 0);
    controller* cont=0;
    retrieve(cont, values["controller"], 0);
    cont->pre_set();
    update();
    index=0;
    while(cont->is_good()){
        sys->simulate();
        cont->control(0, 0);
        write_dat();
        update();
        index++;
    }
    fft_cleanup();
}
