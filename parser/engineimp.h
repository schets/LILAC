#ifndef ENGINEIMP_H
#define ENGINEIMP_H
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "item.h"
class engine;
class engineimp{
    
    std::map<std::string, item*> values;
    void read(std::ifstream& fstr);
    public:
    void run();
    engineimp(const std::string fname);
    ~engineimp();
    friend class engine;
};

#endif
