#include "engineimp.h"
#include "engine.h"
#include <iostream>
#include "utils/comp_funcs.h"
#include "rhs/rhs.h"
#include "rhs/rhs_imp.h"
#include "integrator/integrator.h"
#include "controller/controller.h"
#include "simulation/simulation.h"
#include <time.h>
#include <stdio.h>
#include <cmath>
void engineimp::run(){
    simulation* sys;
    values["simulation"]->retrieve(&sys, 0);
    controller* cont;
    values["controller"]->retrieve(&cont, 0);
    cont->pre_set();
    update();
    while(cont->is_good()){
        double score = sys->simulate();
        cont->control(0, 0);
        update();
    }
    fftw_cleanup();
}
