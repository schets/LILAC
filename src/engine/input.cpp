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
#include "input.h"

input::input(std::map<std::string, item_wrapper >& values):invals(&values){}

void input::insert_item(std::shared_ptr<item> inval){
    if(inval.use_count()){
        if(invals->count(inval->name())==0){
            invals->insert(std::pair<std::string, std::shared_ptr<item>>(
                        inval->name(), inval));
        }
        else{
            err(std::string("Item ") + inval->name() + " already exists in the engine",
                    "input::insert_item", "engine/input.cpp",
#ifdef DEBUG
                    FATAL_ERROR);
#else
                    WARNING);
#endif
        }
    }
    else{
        err("Null item inserted into input map", "input::insert_item",
                "engine/input.cpp", FATAL_ERROR);
    }
}

item_wrapper& input::get_val(const std::string& value){
    auto&& d_it = re_directions.find(value);
    if(d_it != re_directions.end()){
        return d_it->second;
    }
    auto&& m_it = re_mappings.find(value);
    if(m_it != re_mappings.end()){
        return get_val(m_it->second);
    }
    return (*invals)[value];
}

void input::add_redir(const std::string& name, const item_wrapper& inv){
    re_directions[name] = inv;
}

void input::add_remap(const std::string& newname, const std::string& oldname){
    re_mappings[oldname] = newname;
}
