#ifndef graph_h
#define graph_h
#include "engine/item.h"
class graph{
    class graphnode{
        public:
            std::list<std::string> pointsto;
            std::list<std::string> pointedat;
            std::shared_ptr<item> p;
            char processed;
    };
    std::map<std::string, graphnode> nodes;
    void ins_extra_deps();
    void proc_node_deps(graphnode& g);
    //void set_pointedat();
    graphnode& _insert(item_wrapper p);
    public:
    void insert(item_wrapper p);
    std::list<item_wrapper> sort(const std::list<item_wrapper >& l);
};
#endif
