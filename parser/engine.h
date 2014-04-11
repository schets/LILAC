#ifndef ENGINE_H
#define ENGINE_H
#include "item.h"
class engineimp;
class engine{
    engineimp* eng;
    public:
    engine(const std::string fname, const std::string outname="data_out.out", std::string start_ind="0"); 
    ~engine();
    void run();
    void output();
};
#endif
