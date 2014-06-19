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
#ifndef DATA_H
#define DATA_H
#include "utils/defs.hpp"
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <memory>
#include <functional>
//!class to hold a piece of constant data, enforces constant correctness
class data{
    //utility typedefs
	std::string name;
	std::stringstream dat;
//for some reason icc 14 on nersc won't ;et this be a template parameter, and claims std::function doesn't exist...
	template<class T>
	using strfnc = std::function<std::string(const T&)>;//std::string(*strfnc)(const T&);
	template<class T>
		inline void addelem(const T& inval){
			dat << inval << " ";
		}
	template<class T>
		inline void addelem(const T& inval, const strfnc<T>& tostr){
			dat << tostr(inval) << " ";
		}


	friend class writer;

	//workaround of C++ being stupid with shared pointers and private constructors

	struct this_is_private{};
	public:
	//not a template but might as well eliminate the cpp file
	virtual std::string to_string() const{
		return name + ": " + dat.str();
	}
	//instead of exposing constructors directly, call through creation functions
	//to enforce unmodifiable data
	template<class T>
		static inline std::shared_ptr<const data> create(const std::string name, const T& inval){
			return std::make_shared<const data>(this_is_private(), name, inval);
		}
	template<class T>
		static inline std::shared_ptr<const data> create(const std::string _name, T* invals, size_t len){
			return std::make_shared<const data>(this_is_private(), _name, invals, len);
		}
	template<class T>
		static inline std::shared_ptr<const data> create(const std::string name, T* b,
				T* e){
			return std::make_shared<const data>(this_is_private(), name, b, e);
		}
	//custom string function creators
	template<class T>
		static inline std::shared_ptr<const data> create(const std::string name, const T& inval, const strfnc<T>& tostr){
			return std::make_shared<const data>(this_is_private(), name, inval, tostr);
		}
	template<class T>
		static inline std::shared_ptr<const data> create(const std::string _name, T* invals, size_t len, const strfnc<T>& tostr){
			return std::make_shared<const data>(this_is_private(), _name, invals, len, tostr);
		}
	//public but not really constructors
	template<class T>
		data(const this_is_private& dummy, const std::string _name, T* invals, size_t len):name(_name){
			std::for_each(invals, invals + len, [&](const T& val){
					addelem(val);
					});
		}
	template<class T>
		data(const this_is_private& dummy, const std::string& _name, const T& inval):name(_name){
			addelem(inval);
		}
	//constructors taking a custom string function
	template<class T>
		data(const this_is_private& dummy, const std::string _name, T* invals, size_t len, const strfnc<T>& tostr):name(_name){
			std::for_each(invals, invals+len, [&](const T& val){
					addelem(val, tostr);
					});
		}
	template<class T>
		data(const this_is_private& dummy, const std::string& _name, const T& inval, const strfnc<T>& tostr):name(_name){
			addelem(inval, tostr);
		}

#endif
	virtual ~data(){}
};
