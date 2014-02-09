#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "defs.h"
#include "../parser/item.h"
#include "objective.h"
#include <vector>
class toroidal;
//!This is the base class for the control type classes
/*!
 * This class provides the base for classes that execute control algorithms
 * It stores the variables in memory and executes control on them
 */
class controller:public item_dim{
    protected:
    std::vector<variable*> vars;
    public:
    virtual void parse(std::string inval);
    virtual std::vector<std::string> dependencies() const;
    virtual void postprocess(std::map<std::string, item*>& inval);
    virtual void addvar(variable* v)=0;
    virtual std::string type() const = 0;
    virtual void control(comp* u, objective* obj) = 0;
    static item* create(std::string name);
};






#endif


