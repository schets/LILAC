/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
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
    virtual void postprocess(input& dat);
    virtual std::vector<std::string> dependencies() const = 0;
};
#endif
