#ifndef ITEM_DIM_HPP
#define ITEM_DIM_HPP
#include "item.h"
#include "mempool.hpp"
//class that describes items with a dimension, and contains a special memory pool for doing so
class item_dim:public item{
    protected:
        virtual void _do_mem_update();
        mempool memp;
    public:
        size_t dimension;
        inline void update_dim(size_t dim_new){
            dimension = dim_new;
            _do_mem_update();
        }
        virtual void postprocess(std::map<std::string, std::shared_ptr<item>>& dat);
        virtual std::vector<std::string> dependencies() const = 0;
};
#endif
