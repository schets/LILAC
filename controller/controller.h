#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "defs.h"
#include "../parser/item.h"
#include "objective.h"
#include <vector>
//!This is the base class for the control type classes
/*!
 * This class provides the base for classes that execute control algorithms
 * It stores the values in memory 
 */
class controller:public item_dim{
    std::vector<variable*> vars;
    public:
    virtual void parse(std::string inval);
    virtual void postprocess(std::map<std::string, item*>& inval);
    virtual std::string type() const;
    virtual void control(comp* invals, objective* obj) = 0;
};






#endif


