#ifndef graph_h
#define graph_h
#include "item.h"
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
    graphnode& _insert(std::shared_ptr<item> p);
    public:
    void insert(std::shared_ptr<item> p);
    std::list<std::shared_ptr<item>> sort(const std::list<std::shared_ptr<item> >& l);
};
#endif
