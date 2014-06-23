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
#ifndef DEF_H
#define DEF_H
#ifdef CLANG
//this allows Eigen to compile under -Wall
#pragma clang diagnostic ignored "-Wdeprecated-register"
//Not every function uses the full interface
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include <complex>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
//some commands to help with effeciency
#ifdef ICC
#define restr restrict
#define ALIGNED(x) __assume_aligned(x, 16)
#define MAKE_ALIGNED __declspec(align(16))
#define PREDICT(x, val) __builtin_expect(x, val)
#endif

#ifdef CLANG
#define restr __restrict__
#define ALIGNED(x)
#define PREDICT(x, val) x
#define MAKE_ALIGNED alignas(16)
#endif

#ifdef GCC
#define restr __restrict__
#define ALIGNED(x)
#define PREDICT(x, val) __builtin_expect(x, val)
#define MAKE_ALIGNED
#define alignas(x)
#endif
class item;
typedef std::complex<double> comp;
const static comp Id = comp(0, 1);
//#define NUM_TIME_STEPS (256) //number of time points
//#define LENGTH_T (60) //length of t-domain
//parameters taken from matlab main file
//can move to input parameters as well
//but for now these are defines
constexpr double D =-.4;
constexpr double K =(.1);
constexpr double A =(2.0/3.0);
constexpr double B =(1.0/3.0); //need to find a real value for this
constexpr double tau= (.1);
constexpr double Gamma= (.1);
constexpr double RTlength= (1.5);
constexpr double PI =(3.14159);
class item;
class _fatal{
    public:
        _fatal(){};
};
class _warning{
    public:
        _warning(){};
};
const static _fatal FATAL_ERROR;
const static _warning WARNING;
void err(std::string message, std::string function, std::string file, _fatal f);
void err(std::string message, std::string function, std::string file, _warning w);
void err(std::string message, std::string function, std::string file, const item* p, _fatal f);
void err(std::string message, std::string function, std::string file, const item* p, _warning f);
void err(std::string message, std::string function, std::string file, std::shared_ptr<item> p, _fatal f);
void err(std::string message, std::string function, std::string file, std::shared_ptr<item> p, _warning f);


void trim(std::string& curline, const std::string& val = "\n\r\t ");
void ltoken(std::string& tok, std::string& str, std::string delim=" ");
std::string get_unique_name(std::string base = "");
#endif
