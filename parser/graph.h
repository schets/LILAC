#ifndef graph_h
#define graph_h
#include <list>
#include "item.h"
class graph{
    class graphnode{
        public:
            std::list<std::string> pointsto;
            std::list<std::string> pointedat;
            item* p;
            char processed;
    };
    std::map<std::string, graphnode> nodes;
    void ins_extra_deps();
    void proc_node_deps(graphnode& g);
    //void set_pointedat();
    graphnode& _insert(item* p);
    public:
    void insert(item* p);
    int sort(std::list<item*>& l);
    const static int SORT_FAILED=-1;
    const static int SORT_SUCCESS=1;
};
#endif