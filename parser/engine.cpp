#include "engine.h"
#include "engineimp.h"
#include "graph.h"
#include "item.h"
#include <stdlib.h>
void engine_exit(const std::string reason){
    std::cout << "\n\nExiting because: " << reason << std::endl;
    exit(EXIT_FAILURE);
}
char f_is_empty(std::ifstream& fstr){
    return fstr.peek()==std::ifstream::traits_type::eof();
}
engineimp::engineimp(const std::string fname){
    std::ifstream fstr(fname.c_str());
    if(f_is_empty(fstr)){
        engine_exit("Empty/non-existant file passed as configuration parameter");
    }
    read(fstr);
};





/*
 *
 *Engine wrapper functions
 *
 *
 *
 *
 */
engine::engine(const std::string fname):eng(new engineimp(fname)){
}
engine::~engine(){
    if(eng){
        delete eng;
    }
}
