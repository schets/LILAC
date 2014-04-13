#include <time.h>
#include "utils/comp_funcs.hpp"
#include "integrator/integrator.h"
#include "parser/engine.h"
#include <chrono>
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
    srand(time(0));
    std::chrono::high_resolution_clock::time_point tval = std::chrono::high_resolution_clock::now();
    engine e("infile.in", outfile, index);
    std::chrono::duration<double> init_dur = std::chrono::duration_cast<std::chrono::duration<double> >(std::chrono::high_resolution_clock::now() - tval);
    std::cout << "Initialization took " << init_dur.count() << " second" << std::endl;
    e.run();
    //
    //This cleans up the fftw memory
    //not necessary since memory is freed on program exit
    //but makes finding memory leaks far far easier
    //fftw_cleanup();
    return 0;
}
