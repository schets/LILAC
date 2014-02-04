#ifndef SYSTEM_H
#define SYSTEM_H
#include "defs.h"
#include "parser/item.h"
#include "controller/controller.h"
#include "objective/objective.h"
/*!
 * This class defines the system being simulated. It simulates a run of the system
 * and returns a single value for the 
 */
class system:public item_dim{
    protected:
        controller* cont;
    public:
        virtual void checkpoint();
        virtual double simulate() = 0;
        virtual void dump()=0;
        virtual void postprocess(std::map<std::string, item*>& inval)=0;
        virtual std::vector<std::string> dependencies() const = 0;
};
#endif
