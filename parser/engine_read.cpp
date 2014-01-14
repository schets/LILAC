#include "engineimp.h"
#include <string>
#include <map>
#include <list>
#include "graph.h"
#include "item.h"
void engineimp::read(std::ifstream& fstr){
    std::string curline;
    std::getline(fstr, curline);
    std::list<item*> post_order;
    while(fstr.good()){
        std::getline(fstr, curline);
    }
};


