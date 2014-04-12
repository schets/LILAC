#ifndef ITEM_DIM_HPP
#define ITEM_DIM_HPP
#include "item.h"
#include "mempool.hpp"
#include <set>
//class that describes items with a dimension, and contains a special memory pool for doing so
class item_dim:public item{
    //This prevents anyone else from calling
    //the uncontrolled update function
    void update_dim(size_t dim_new, size_t dummy);

    protected:
    void print_upstream();
    virtual void _do_mem_update(size_t dim_old);
    mempool memp;
    std::set<item_dim*> children;
    item_dim* parent;
    size_t dimension;
    void add_as_parent(item_dim* p);
    void add_as_parent(std::shared_ptr<item_dim> p);
    void update_dim(size_t dim_new);
    public:
    inline size_t get_dimension() const{
        return dimension;
    }
    virtual void postprocess(std::map<std::string, std::shared_ptr<item>>& dat);
    virtual std::vector<std::string> dependencies() const = 0;
};
#endif
