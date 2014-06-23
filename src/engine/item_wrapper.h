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
#ifndef ITEM_WRAPPER_H
#define ITEM_WRAPPER_H
#include "item.h"
#include <type_traits>
class item;
//!This class wraps the actual item that has been input
/*
 * This class wraps the items that have been created by the system.
 * Items need wrapping to have proper dependency resolution. Internally,
 * all objects are stored in the same map. This means that local objects must have
 * names that differ from global objects. However, they still must be callable from
 * the retrieve and dependency functionality and therefore each item must have a local map
 * from dependency names to actual dependency objects
 */
class item_wrapper{
    std::shared_ptr<item> ptr;
    std::set<std::string> deps;
    public:
    template<class T, class = typename std::enable_if<std::is_base_of<item, T>::value>::type>
	    item_wrapper(const std::shared_ptr<T>& inv){
		    ptr = std::static_pointer_cast<item>(inv);
		    if(ptr.use_count()){
			    for(auto& str : inv->dependencies()){
				    deps.insert(str);
			    }
		    }
	    }
    item_wrapper(const std::shared_ptr<item>& inv);
    item_wrapper(const item_wrapper& inv);
    // item_wrapper(item_wrapper&& inv);
    item_wrapper();
    std::vector<std::string> dependencies() const;
    item* get();
    const item* get() const;
    item* operator ->();
    const item* operator ->() const;
    void add_dep(const std::string& indep);
    void replace_dep(const std::string& newdep, const std::string& olddep, bool must_rep=true);
    operator std::shared_ptr<item>();
    std::shared_ptr<item> get_shared();
    const std::shared_ptr<item> get_shared()const;

};
#endif
