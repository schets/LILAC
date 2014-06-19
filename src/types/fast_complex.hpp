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
#ifndef FAST_COMPLEX_HPP
#define FAST_COMPLEX_HPP
//helper class to eliminate operations
template<class T>
struct imag_unit final{};
template<class T>
struct imag_val final{
    T val;
};
template<class T>
struct fast_comp final{
    T rval;
    T ival;
    public:
    inline fast_comp(){}
    inline fast_comp(const T& r):rval(r){}
    inline fast_comp(const T& r, const T& i):rval(r), ival(i){}
    inline fast_comp(const fast_comp<T>& a):rval(a.rval), ival(a.ival){}
    inline fast_comp<T> operator +(const fast_comp<T>& a){
        return fast_comp<T>(rval+a.rval, ival+a.ival);
    }
    inline fast_comp<T> operator -(const fast_comp<T>& a){
        return fast_comp<T>(rval-a.rval, ival-a.ival);
    }
    inline fast_comp operator *(const fast_comp<T>& a){
    }
};
#endif
