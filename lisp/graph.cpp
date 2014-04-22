#include "engine/engineimp.h"
#include "graph.h"
void graph::insert(item_wrapper  it_p){
    if(nodes.count(it_p->name())){
        std::cout << "Warning: variable " << it_p->name() << "has already been entered\n";
        std::cout <<"Overwriting with new value\n";
    }
    _insert(it_p);
}
graph::graphnode& graph::_insert(item_wrapper it_p){
    graphnode& g = nodes[it_p->name()];
    g.p = it_p;
    g.processed=0;
    std::vector<std::string> deps(it_p.dependencies());
    g.pointsto.clear();
   // g.pointsto.insert(g.pointsto.begin(), deps.begin(), deps.end());
    for(auto& val : deps){
        trim(val);
        g.pointsto.push_back(val);
    }
    return g;
}
void graph::ins_extra_deps(){
    for(auto& node : nodes){
        graphnode& g = node.second;
        if(!g.processed){
            proc_node_deps(g);
        }
    }
}
//may add support for dependencies that aren't
//input variables later if need be
void graph::proc_node_deps(graph::graphnode& g){
    g.processed=1;
    constexpr char maybe_dep = '!';
    constexpr char exist_dep = '#';
    std::list<std::string> gets_removed;
    for(auto& val : g.pointsto){
        //existence is optional
        if(val[0] == maybe_dep){
            //remove the dependency if it doesn't exist
            if(!nodes.count(val)){
                gets_removed.push_back(val);
            }
            else{
                val.erase(0,1);
                nodes[val].pointedat.push_back(g.p->name());
            }
        }
        //postprocesing does not matter for the dependency
        else if(val[0] == exist_dep){
            if(!nodes.count(val)){
                val.erase(0,1);
                std::cout<<g.p->name()<<" depends on "<<val<<", which has not been inserted\n";
                exit(EXIT_FAILURE);
            }
            gets_removed.push_back(val);
        }
        else{
            if(!nodes.count(val)){
                std::cout<<g.p->name()<<" depends on "<<val<<", which has not been inserted\n";
                exit(EXIT_FAILURE);
            }
            nodes[val].pointedat.push_back(g.p->name());
        }
    }
    //remove dependencies that are not considered to exist anymore
    for(const auto& val : gets_removed){
        g.pointsto.remove(val);
    }
}
//returns sorted list, all items in list are added to the graph
std::list<item_wrapper > graph::sort(const std::list<item_wrapper >& l){
    std::list<item_wrapper > outlist;
    std::list<const graphnode*> queuelist;
    for(const auto& val: l){
        insert(val);
    }
    ins_extra_deps();
    //populate the list of nodes without dependencies
    for(const auto& node : nodes){
        const graphnode& g = node.second;
        if(g.pointedat.empty()){
            queuelist.push_back(&g);
        }
    }
    //perform topological sort algorithm
    while(!queuelist.empty()){
        const graphnode& g = *queuelist.front();
        queuelist.pop_front();
        outlist.push_front(g.p);
        for(const auto& val : g.pointsto){
            graphnode& gn = nodes[val];
            gn.pointedat.remove(g.p->name());
            if(gn.pointedat.empty()){
                queuelist.push_back(&gn);
            }
        }
    }
    //check to see if list was fully sorted 
    if(l.size() != outlist.size()){
        err("Topological sort has failed, check for cyclical input dependencies",
                "graph::sort", "engine/graph.cpp", FATAL_ERROR);
    }
    return outlist;
}
