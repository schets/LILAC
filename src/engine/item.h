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
#ifndef ITEM_H
#define ITEM_H
#include <map>
#include <set>
#include <iostream>
#include "utils/defs.hpp"
class engineimp;
class retrieve_wrapper;
class item;

template<class T>
void retrieve(T& inval, item* sender, item* caller);
class input;
class native_item;
class item{
    protected:
    //_retrieve should never be called by itself, only through the retrieve function
    //It doesn't really matter whether retrieve is public/private/whatever,
    //since nothing besides safe_retrieve can construct __retrieve_checkers
    //Any class overloading this should think carefully about what it's purpose is,
    //since anything that doesn't hold a native value is likely better off as an item*
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    std::string _name;
    std::string _write_name;
    char has_write_name;
    public:
    item();
    virtual ~item();
    engineimp* holder;
    static std::shared_ptr<item> create(std::string name, engineimp* rval);
    virtual void print() const;

    virtual void postprocess(input& indat);
    virtual std::vector<std::string> dependencies() const;
    virtual void update();

    virtual std::string type() const = 0;
    void setname(const std::string n);
    void set_write_name(std::string wname);
    const std::string& name()const;
    const std::string& write_name() const;

    template<class T> friend void retrieve(T& inval, item* setter, item* caller);
    friend class input;
    friend class native_item;
};
class native_item:public item{
    protected:
        //move tom protected scope so native values can override it
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    public:
        virtual void parse(const std::string& inval) = 0;
        //native items can't have dependencies or perform postprocessing
        std::vector<std::string> dependencies() const final_def;
        void postprocess(input& inval) final_def;
};
#ifdef ICC
//!helper function for variable class
bool operator < (const std::weak_ptr<item>& a, std::weak_ptr<item>& b);
#endif



//classes for testing graphsort, mainly to ensure that it finds cyclical and unsatisfied dependencies
class ftest1 : public item{
	public:
		virtual std::vector<std::string> dependencies() const{
			std::string deps[] = {"var1"};
			return std::vector<std::string>(deps, deps+1);
		}
		virtual std::string type() const{
			return "ftest1";
		}
};

class ftest2 : public item{
	public:
		virtual std::vector<std::string> dependencies() const{
			std::string deps[] = {"var2"};
			return std::vector<std::string>(deps, deps+1);
		}
		virtual std::string type() const{
			return "ftest2";
		}
};
class ftest3 : public item{
	public:
		virtual std::vector<std::string> dependencies() const{
			std::string deps[] = {"var3", "var4"};
			return std::vector<std::string>(deps, deps+2);
		}
		virtual std::string type() const{
			return "ftest3";
		}
};
#endif

