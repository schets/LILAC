#include "engineimp.h"
#include "graph.h"
#include <iostream>
#include <stdlib.h>
void graph::insert(item* it_p){
    if(nodes.count(it_p->name())){
        std::cout << "Warning: variable " << it_p->name() << "has already been entered\n";
        return;
    }
    _insert(it_p);
}
graph::graphnode& graph::_insert(item* it_p){
    graphnode& g = nodes[it_p->name()];
    g.p = it_p;
    g.processed=0;
    std::vector<std::string> deps = it_p->dependencies();
    g.pointsto.clear();
    g.pointsto.insert(g.pointsto.begin(), deps.begin(), deps.end());
    return g;
}
void graph::ins_extra_deps(){
    std::map<std::string, graphnode>::iterator beg;
    for(beg=nodes.begin(); beg!= nodes.end(); beg++){
        graphnode& g = beg->second;
        if(!g.processed){
            proc_node_deps(g);
        }
    }
}
//may add support for dependencies that aren't
//input variables later if need be
void graph::proc_node_deps(graph::graphnode& g){
    g.processed=1;
    std::list<std::string>::iterator beg;
    for(beg = g.pointsto.begin(); beg != g.pointsto.end(); beg++){
        if(!nodes.count(*beg)){
            std::cout<<g.p->name()<<" depends on "<<*beg<<", which has not been inserted\n";
            exit(EXIT_FAILURE);
        }
            
        nodes[*beg].pointedat.push_back(g.p->name());
    }
}
//returns sorted list, all items in list are added to the graph
int graph::sort(std::list<item*>& l){
    std::list<graphnode*> queuelist;
    std::list<item*>::iterator begl = l.begin();
    for(; begl != l.end(); begl++){
        insert(*begl);
    }
    l.clear(); 
    ins_extra_deps();
    std::map<std::string, graphnode>::iterator beg = nodes.begin();
    //populate the list of nodes without dependencies
    for(; beg != nodes.end(); beg++){
        graphnode& g = beg->second;
        if(g.pointedat.empty()){
            queuelist.push_back(&g);
        }
    }
    //perform topological sort algorithm
    while(!queuelist.empty()){
        graphnode& g = *queuelist.front();
        queuelist.pop_front();
        l.push_back(g.p);
        std::list<std::string>::iterator sbeg = g.pointsto.begin();
        for(; sbeg != g.pointsto.end(); sbeg++){
            graphnode& gn = nodes[*sbeg];
            gn.pointedat.remove(g.p->name());
            if(gn.pointedat.empty()){
                queuelist.push_back(&gn);
            }
        }
        g.pointsto.clear();
    }
    //check to see if any edges remain in the graph
    for(beg=nodes.begin(); beg != nodes.end(); beg++){
        graphnode& g = beg->second;
        if(!g.pointedat.empty()){
            return SORT_FAILED;
        }
    }
    return SORT_SUCCESS;
}
int main(){
    graph g;
    engineimp ee("testfile.in");
    return 0;
}
