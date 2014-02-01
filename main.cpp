#include <iostream>
#include <time.h>
#include "defs.h"
#include "comp_funcs.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
#include <parser/engine.h>
using namespace std;
int main(int argc, char** argv){
    engine e("infile.in");
    e.run();
    //
    //This cleans up the fftw memory
    //not necessary since memory is freed on program exit
    //but makes finding memory leaks far far easier
    //fftw_cleanup();
    return 0;
}
