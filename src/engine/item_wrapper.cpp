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
#include "item_wrapper.h"
item* item_wrapper::operator->(){
    return get();
}
const item* item_wrapper::operator->()const{
    return get();
}
std::vector<std::string> item_wrapper::dependencies()const{
    return std::vector<std::string>(deps.begin(), deps.end());
}
//!Adds a dependency to the wrapper
void item_wrapper::add_dep(const std::string& indep){
    deps.insert(indep);
}
//!Replaces an existing dependency with a new one
void item_wrapper::replace_dep(const std::string& newdep, const std::string& olddep, bool must_rep){
    if(deps.erase(olddep) || !must_rep){
        deps.insert(newdep);
    }
}
item* item_wrapper::get(){
    if(ptr.use_count()){
        return ptr.get();
    }
    return 0;
}
const item* item_wrapper::get()const{
    if(ptr.use_count()){
        return ptr.get();
    }
    return 0;
}
item_wrapper::operator std::shared_ptr<item>(){
    return ptr;
}
std::shared_ptr<item> item_wrapper::get_shared(){
    return ptr;
}
const std::shared_ptr<item> item_wrapper::get_shared()const{
    return ptr;
}
item_wrapper::item_wrapper(const std::shared_ptr<item>& inv){
    ptr = inv;
    if(ptr.use_count()){
        for(auto& str : inv->dependencies()){
            deps.insert(str);
        }
    }
}

item_wrapper::item_wrapper(const item_wrapper& inv):ptr(inv.ptr),
    deps(inv.deps){
    }
item_wrapper::item_wrapper(){
}
