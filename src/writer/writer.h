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
#ifndef WRITER_H
#define WRITER_H
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include "utils/defs.hpp"
#include "data.hpp"
//!This class defines the interface for sending data to the engine for writing
class writer{
    //!Holds each data point
    std::list<std::shared_ptr<const data>> dvals;
    //!Maps enum parameters to a bool deciding whether that label will write or not
    std::map<int, bool> will_write;
    friend class engineimp;
    //!Writes the current data to a file
    static bool is_w_type(int inval);
    public:
    //!Writes to the ostream and returns the size of the data written
    virtual size_t write(std::ostream& ofile) const;
    //!Labels for various input types
    typedef enum{
        FINAL_SCORE,
        FINAL_FUNC,
        INTERMEDIATE_FUNC,
        INTERMEDIATE_SCORE,
        INDEX,
        PARAMETER,
        OTHER,
        BEGIN=FINAL_SCORE,
        END=OTHER} w_type;

    virtual void add_data(std::shared_ptr<const data> inval, const w_type dat_t);
    std::string to_string() const;
    void set_rule(w_type inval, bool rule);
    virtual ~writer();
    writer(bool def = true);
    writer(const std::map<int, bool>& writevals, bool def = true);
};
#endif
