#pragma once
#ifndef DATA_H
#include "utils/defs.hpp"
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
//!class to hold a piece of constant data, enforces constant correctness
class data{
    //utility typedefs
    template<class T>
        using strfnc = std::function<std::string(T)>;
    template<class T>
        using in_iter=std::iterator<std::input_iterator_tag, T>;
    std::string name;
    std::stringstream dat;
    template<class T>
        inline void addelem(const T& inval){
            dat << inval << " ";
        }
   template<class T>
        inline void addelem(const T& inval, const strfnc<T>& tostr){
            dat << tostr(inval) << " ";
        }
    friend class writer;
    template<class T>
        data(const std::string _name, T* invals, size_t len): data(_name, invals, invals+len){}
    template<class T>
        data(const std::string& _name, const T& inval):name(_name){
            addelem(inval);
        }
    template<class T>
        data(const std::string _name, in_iter<T> b, in_iter<T> e):name(_name){
            std::for_each(b, e, [this](const T& val){addelem(val);});
        }
    //constructors taking a custom string function
    template<class T>
        data(const std::string _name, T* invals, size_t len, const strfnc<T>& tostr): data(_name, invals, invals+len, tostr){}
    template<class T>
        data(const std::string& _name, const T& inval, const strfnc<T>& tostr):name(_name){
            addelem(inval, tostr);
        }
    template<class T>
        data(const std::string _name, in_iter<T> b,
                in_iter<T> e, const strfnc<T>& tostr):name(_name){
            std::for_each(b, e, [&](const T& val){addelem(val, tostr);});
        }

    public:
    //instead of exposing constructors directly, call through creation functions
    //to enforce unmodifiable data
    std::string to_string() const;
    template<class T>
        static const inline data* create(const std::string name, const T& inval){
            return new data(name, inval);
        }
    template<class T>
        static const inline data* create(const std::string _name, T* invals, size_t len){
            return new data(_name, invals, len);
        }
    template<class T>
        static const inline data* create(const std::string name, in_iter<T> b,
                in_iter<T> e){
            return new data(name, b, e);
        }
    //custom string function creators
    template<class T>
        static const inline data* create(const std::string name, const T& inval, const strfnc<T>& tostr){
            return new data(name, inval, tostr);
        }
    template<class T>
        static const inline data* create(const std::string _name, T* invals, size_t len, const strfnc<T>& tostr){
            return new data(_name, invals, len, tostr);
        }
    template<class T>
        static const inline data* create(const std::string name, in_iter<T> b,
                in_iter<T> e, const strfnc<T>& tostr){
            return new data(name, b, e, tostr);
        }
};
#define DATA_H
#endif
