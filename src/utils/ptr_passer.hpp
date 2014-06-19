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
#pragma once
#ifndef PTR_PASSER_HPP
#define PTR_PASSER_HPP
#include "defs.hpp"
class ptr_passer{
    const std::type_info& tval;
    void* _ptr;
    public:
    template<class T>
        ptr_passer(T* inptr):tval(typeid(T)), _ptr(inptr){
#ifdef DEBUG
            if(typeid(T)==typeid(void)){
                err("A ptr_passer is being used to pass a void pointer",
                        "ptr_passer/get_ptr.hpp", "utils/ptr_passer.hpp", WARNING);
            }
#endif
        };
    ptr_passer(const ptr_passer& inval):tval(inval.tval), _ptr(inval._ptr){
    }
    template<class T>
        inline operator T*(){
            if(typeid(T) != tval){
                err(std::string("A pointer of type ") + typeid(T).name() +
                        " has been requested from a ptr_passer of type " + tval.name(),
                        "ptr_passer/get_ptr.hpp", "utils/ptr_passer.hpp", FATAL_ERROR);
            }
            return static_cast<T*>(_ptr);
        }
};
#endif
