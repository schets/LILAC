#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../parser/item.h"
#include "parser/item_dim.h"
#include "objective/objective.h"
//!This is the base class for the control type classes
/*!
 * This class provides the base for classes that execute control algorithms
 * It stores the variables in memory and executes control on them
 */
class controller:public item_dim{
    protected:
    std::vector<std::weak_ptr<variable> > vars;
    public:
    int index;
    size_t num_cont;
    virtual void parse(std::string inval);
    virtual std::vector<std::string> dependencies() const;
    virtual void postprocess(std::map<std::string, std::shared_ptr<item> >& inval);
    virtual void addvar(std::weak_ptr<variable> v)=0;
    virtual std::string type() const = 0;
    virtual void control(comp* u, objective* obj) = 0;
    virtual char is_good() = 0;
    virtual void pre_set(){};
    static item* create(std::string name);
};
#endif


