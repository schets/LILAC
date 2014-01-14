#ifndef ENGINE_H
#define ENGINE_H
#include <string>
#include "item.h"
class engineimp;
class engine{
    engineimp* eng;
    public:
    engine(const std::string fname); 
    ~engine();
    void run();
    void output();
};
#endif
