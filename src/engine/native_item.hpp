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
#ifndef NATIVE_ITEM_HPP
#define NATIVE_ITEM_HPP
#include "item.h"
#include "utils/retrieve_checker.hpp"

//!This class provides a generic interface for making native types
/**
 * This class provides a method for creating a generic type.
 * The parameter T determines what the type is, and the string act_name
 * determines the type name
 */
template<class T, const char act_name[]>
class native_tmpl:public native_item{
    protected:
    T value;
    void _retrieve(retrieve_wrapper&& retval, item* caller){
        retval.check_and_get_type(typeid(T), &value);
    }
    public:
    std::string type() const{
        return act_name;
    }
    void parse(const std::string& inval){
        std::stringstream parser(inval);
        parser >> value;
    }

};

namespace __HIDER__{
    struct native_names{
        public:
            static constexpr char double_name[] = "double";
            static constexpr char float_name[] = "float";
            static constexpr char unsigned_name[] = "unsigned";
            static constexpr char string_name[] = "string";
            static constexpr char integer_name[] = "integer";
            static constexpr char vector_name[] = "vector";
    };
}
typedef native_tmpl<double, __HIDER__::native_names::double_name> _double;
typedef native_tmpl<float, __HIDER__::native_names::float_name> _float;
typedef native_tmpl<std::string, __HIDER__::native_names::string_name> string;
typedef native_tmpl<size_t, __HIDER__::native_names::unsigned_name> _unsigned;
typedef native_tmpl<int, __HIDER__::native_names::integer_name> integer;
#endif
