#pragma once
#ifndef DATA_H
#define DATA_H
#include "utils/defs.hpp"
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <memory>
//!class to hold a piece of constant data, enforces constant correctness
class data{
    //utility typedefs
    template<class T>
        using strfnc = std::function<std::string(T)>;
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
    template<class in_iter, class end_iter>
        static inline std::shared_ptr<const data> create(const std::string name, in_iter b,
                end_iter e){
            return std::make_shared<const data>(this_is_private(), name, b, e);
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
    template<class T, class in_iter, class end_iter>
        static inline std::shared_ptr<const data> create(const std::string name, in_iter b,
                end_iter e, const strfnc<T>& tostr){
            return std::make_shared<const data>(this_is_private(), name, b, e, tostr);
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
    template<class in_iter, class end_iter, class T>
        data(const this_is_private& dummy, const std::string _name, in_iter b, end_iter e):name(_name){
            std::for_each(b, e, [this](const T& val){addelem(val);});
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
