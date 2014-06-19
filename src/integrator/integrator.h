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
#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP
#include "../rhs/rhs.h"
#include "../engine/item.h"
#include "engine/item_dim.h"
#include "utils/vartype.hpp"
#include "utils/ptr_passer.hpp"
//!This class defines the integration base class
/*!
 * This class defines the base integration class, the interface that is used by
 * the engine to integrate differential equations
 */
class integrator:public item_dim, public vartype{
    protected:
        rhs* rh_val;
    public:
        virtual void initial_condition(ptr_passer in, size_t len);
        inline void initial_condition(ptr_passer in){
            initial_condition(in, dimension);
        }
        virtual void postprocess(input& inval);
        virtual std::vector<std::string> dependencies() const = 0;
        //!Returns the name of the integrator
        virtual std::string type() const = 0;
        //!Integrates the function rh
        /*!
         * This function integrates the equation du/dt=rh->dxdt(u) from t0 to tf
         * @param rh The function being integrated
         * @param u A pointer to the initial condition, also stores the final condition
         * @param t0 The initial time
         * @param tf The ending time
         */
        virtual int integrate(ptr_passer u, double t0, double tf) = 0;
        //!Blank destructor for integrator
        virtual ~integrator(){}
};

#endif
