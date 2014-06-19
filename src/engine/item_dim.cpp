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
#include "item_dim.h"
#include "utils/item_heads.hpp"
void item_dim::_do_mem_update(size_t dim_old){
    memp.set_dim(dimension);
}
/*
 * Postprocesses the item_dim class, which sets the dimension to the input variable dimension
 */
void item_dim::postprocess(input& dat){
    int dimt;
    parent=0;
    dat.retrieve(dimt, "dimension", this);
    if(dimt <= 0){
        std::string errmess = "dimension invalid, must be >= 0";
        err(errmess, "int_dim::postprocess", "item/item.cpp",
                dat["dimension"], FATAL_ERROR);
    }
    dimension = (size_t)dimt;
}

/*!
 * Returns the dependencies of item_dim
 * \note
 * item_dim strongly depends on the input value integer dimension
 * @return Vector containing the dependencies of item_dim
 */
std::vector<std::string> item_dim::dependencies()const{
    std::vector<std::string> de;
    de.push_back("dimension");
    return de;
}


void item_dim::add_as_parent(item_dim* p){
    if(parent){
        err(this->name() + " is already a child of " +
                parent->name() + ", not adding as a child of " +
                p->name(), "item_dim::add_as_parent", "engine/item_dim.cpp",
                WARNING);
    }
    else{
        p->children.insert(this);
        parent=p;
        if(dimension != p->dimension){
            dimension=p->dimension;
        err(std::string() + "Changing dimension of "+ name() + " to match " + p->name(),
                "item_dim::add_as_parent", "engine/item_dim.cpp", WARNING);
        }
    }
}

void item_dim::add_as_parent(std::shared_ptr<item_dim> p){
    add_as_parent(p.get());
}


void item_dim::update_dim(size_t dim_new){
    if(parent){
        err(std::string("Item_dim* ") + this->name() + " is not the head of the \
                dimension dependency tree", "item_dim::update_dim(size_t)", 
                "engine/item_dim.cpp", WARNING);
        print_upstream();
    }
    else{
        update_dim(dim_new, 0);
    }
}
void item_dim::update_dim(size_t dim_new, size_t dummy){
    size_t dim_old = dimension;
    dimension = dim_new;
    _do_mem_update(dim_old);
    for(auto child : children){
        child->update_dim(dim_new, dummy);
    }
}
void item_dim::print_upstream(){
    std::cout << this->name();
    if(parent){
        std::cout << " -> ";
        parent->print_upstream();
    }
    else{
        std::cout << std::endl;
    }
}
