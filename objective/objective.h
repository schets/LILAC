#ifndef OBJECTIVE_H
#define OBJECTIVE_H
#include "parser/item.h"
#include "parser/item_dim.h"
class objective:public item_dim{
    public:
        virtual double score(comp* invals) = 0;
        virtual std::vector<std::string> dependencies() const=0;
        virtual void postprocess(input& inval)=0;
        virtual void parse(std::string inval){};
        static item* create(std::string nameval);
};
#endif
