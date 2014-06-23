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
#ifndef stable_h
#define stable_h
#include "simulation.h"
#include "integrator/integrator.h"
//!A system that attempts to reach an equilibrium
/*! This class defines a system that tries to reach an equilibrium through repeated iterations
 */

class stable:public simulation, public vartype{
    protected:
        //!Maximum number of times the system is iterated before assuming an unstable state
        int max_iterations;
        //!The threshold of change between two iterations for which the system will be considered stable
        double change_threshold;
        //!Returns the difference between the two most recent iterations
        virtual double get_change() = 0;
        //iterates the system forwards in time
        virtual void iterate_system() = 0;
        virtual std::vector<std::string> dependencies() const = 0;
        virtual void postprocess(input& invals) = 0;
        int num_gone;
        int round;
        int bad_res;
        std::shared_ptr<writer> cur_writer;
    public:
        double simulate();
        virtual ~stable(){};
};
//!A class representing an ODE system that is integrated towards stability
/*!
 * This class represents an ODE system that undergoes integration, possibly towards a stable equilibrium.
 * Upon reaching this equilibrium, the integration will cease.
 *
 * The class tests this by doing multiple sub-integrations,
 * and checking the difference between consecutive solutions.
 * In addition, operators can be applied before and after each integration round.
 *
 * Note that this class is really a wrapper to a statically typed template class.
 * This allows the system to choose the type at runtime, and generally be ignorant of the
 *  variable type of the defined problem. 
 */
class stable_ode:public stable{
    protected:
        std::shared_ptr<stable_ode> actual;
        //!This defines the integrator being used
        integrator* inter;
        //T!his applies operations before the integration occurs
        virtual void pre_integration_operations();
        //!This applies operations ofter the integration occurs
        virtual void post_integration_operations();
        virtual void iterate_system();
        virtual double get_change();
    public:
        const std::type_info& vtype() const;
        double score();
        virtual std::vector<std::string> dependencies() const;
        virtual void postprocess(input& invals);
        virtual std::string type() const;
        virtual ~stable_ode();
};

#endif
