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
#ifndef RHS_TYPE_H
#define RHS_TYPE_H
#include "rhs.h"
//!Type interface for the rhs class
/*!
 *
 * This class associates each rhs object with a given type
 * This make it possible to allow for runtime selection of typed integrators
 * while using a template to help save code. Each general purpose integrator should always support
 * a type that has the basic numeric operators and an abs function defined.
 * This should be enough for almost all
 * cases but you can define your own if need be
 *
 * \sa rk45, \sa rhs, \sa rhs_CNLS, \sa rk45, \sa rk45_tmpl
 */
template<typename T, class _rhs_type=rhs> class rhs_type:public _rhs_type{
    public:
        //!returns the type of the current variable
        virtual const std::type_info& vtype() const final_def{
            return typeid(T);
        }
        
        //!Generic initial condition, call default constructor for each element/
        virtual void initial_condition(ptr_passer in, size_t len){
//ICC doesn't support default_constructible
#ifndef ICC
            static_assert(std::is_default_constructible<T>::value,
                    "Type needs to be default constructable in rhs_type");
#endif
            T* vals = in;
            for(size_t j = 0; j < len; j++){
                vals[j] = T();
            } 
        }
};
#endif
