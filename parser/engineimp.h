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
    char item_exists(std::string name) const;
    engineimp(const std::string fname);
    ~engineimp();
    friend class engine;
    friend class item;
};

#endif
