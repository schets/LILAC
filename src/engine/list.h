#ifndef LIST_H
#define LIST_H
/*
 * This is just a base for an actual list implementation in the engine
 * The whole variable class needs an overhaul as well
 *
 */
#include "item.h"
class list : public native_item{
    std::vector<native_item*> vals;
    public:
    std::string type() const;
    void print() const;
    void parse(const std::string& inval);
    void _retrieve(retrieve_wrapper&& inval, item* caller);
};
#endif
