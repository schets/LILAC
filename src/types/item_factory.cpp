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
#include "item_factory.h"
#include <map>
#include "defs.hpp"
typedef std::function<item*()> create_fnc; 
std::map<std::string, create_fnc>* types=0;
void item_factory::register_type(const std::string& name, create_fnc creator){
    //this enforces that types will be made upon entry into the function
    static std::map<std::string, create_fnc> make_types;
    types = &make_types;
    if(types->count(name)){
        err(name + " has already been registered in the type system",
                "item_factory::register_type", "utils/item_factory.cpp",
                FATAL_ERROR);
    }
    types->insert(std::pair<std::string, create_fnc>(name, creator));
}
item* item_factory::create_item(const std::string& name){
    if(!types || !types->count(name)){
        err(std::string() + "The requested type " + name + " does not exist",
                "item_factory::create_item", "utils/item_factory.cpp",
                FATAL_ERROR);
    }
    return (*types)[name]();
}
