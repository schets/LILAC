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
#include "writer.h"

/*!
 * This functions adds the data to the list of values to be written,
 * as long as the rule corresponding to the type is set to true
 *
 * @param inval The data to be saved
 * @param dat_t The type of the input data
 */
void writer::add_data(std::shared_ptr<const data> inval, const w_type dat_t){
    if(will_write[dat_t]){
        dvals.push_back(inval);
    }
}
/*!
 * This functions sets the rule for the input type
 * @param inval The type which is having a rule reset
 * @param rule the new rule for the type
 */
void writer::set_rule(w_type inval, bool rule){
    will_write[inval] = rule;
}

/*!
 * This creates a new writer class with all of the type rules set to def
 * @param def The default rule for all of the types
 */
writer::writer(bool def){
    for(int i = BEGIN; i <= END; i++){
        will_write[i] = def;
    }
}

/*!
 * This constructs a writer that has the same rules as the map writevals, with any
 * rule that isn't in writevals set to def
 *
 * @param writevals The current set of rules being used
 * @param def The default rule for values not mapped by writevals
 */
writer::writer(const std::map<int, bool>& writevals, bool def):writer(def){
    for(auto val : writevals){
        if(is_w_type(val.first)){
            will_write[val.first] = val.second;
        }
    }
};

writer::~writer(){}

bool writer::is_w_type(int inval){
    for(int i = BEGIN; i < END; i++){
        if(inval == i){
            return true;
        }
    }
    return false;
}


size_t writer::write(std::ostream& ofile) const{
    size_t total_size = 0;
    for(const auto& val : dvals){
        std::string&& temp_str = val->to_string();
        total_size += temp_str.size() * sizeof(std::string::value_type);
        ofile << temp_str;
        ofile << '\n';
        total_size += sizeof('\n');
    }
    return total_size;
}

std::string writer::to_string() const{
	std::stringstream val;
	write(val);
        return val.str();
}
