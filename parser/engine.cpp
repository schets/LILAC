#include "engine.h"
#include "engineimp.h"
#include "item.h"
#include "defs.h"
#include <stdlib.h>
char f_is_empty(std::ifstream& fstr){
    return fstr.peek()==std::ifstream::traits_type::eof();
}
engineimp::engineimp(const std::string fname){
    std::ifstream fstr(fname.c_str());
    if(f_is_empty(fstr)){
        err("Empty/non-existant file passed as configuration parameter",
                "engineimp::engineimp(std::string)", "parser/engine.cpp", FATAL_ERROR);
    }
    read(fstr);
};

engineimp::~engineimp(){
    std::map<std::string, item*>::iterator beg;
    for(beg=values.begin(); beg != values.end(); beg++){
        if(beg->second){
      //      std::cout << "Starting deletion for " << beg->second->name() << std::endl;
            delete beg->second;
        }
    }
}

char engineimp::item_exists(item* p) const{
    if(!p){
        return 1;
    }
    std::map<std::string, item*>::const_iterator mbeg;
    for(mbeg = values.begin(); mbeg != values.end(); mbeg++){
        if(p == mbeg->second){
            return 1;
        }
    }
    return 0;
}

/*
 *
 *Engine wrapper functions
 *
 *
 *
 *
 */

/*!
 * Constructs an engine object, a wrapper for engimeimp
 * \sa engineimp
 */
engine::engine(const std::string fname){
    eng = new engineimp(fname);
}
engine::~engine(){
    if(eng){
        delete eng;
    }
}
void engine::run(){
    eng->run();
}
