#include <iostream>
#include <time.h>
#include "utils/defs.h"
#include "utils/comp_funcs.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
#include "parser/engine.h"
using namespace std;
int main(int argc, char** argv){
    std::string outfile;
    std::string index;
    if(argc == 2){
        outfile = argv[1];
        std::cout << outfile << ", " << argv[1] << std::endl;
    }
    else if(argc == 3){
        outfile = argv[1];
        index = argv[2];
    }
    else{
        outfile = "data_out.out";
        index="0";
    }
    engine e("infile.in", outfile, index);
    e.run();
    //
    //This cleans up the fftw memory
    //not necessary since memory is freed on program exit
    //but makes finding memory leaks far far easier
    //fftw_cleanup();
    return 0;
}
