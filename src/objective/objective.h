#ifndef OBJECTIVE_H
#define OBJECTIVE_H
#include "engine/item.h"
#include "engine/item_dim.h"
class objective:public item_dim{
    public:
        virtual double score(comp* invals) = 0;
        virtual std::vector<std::string> dependencies() const=0;
        virtual void postprocess(input& inval)=0;
};
#endif
