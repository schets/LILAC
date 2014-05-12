#pragma once
#ifndef RETRIEVE_CHECKER_HPP
#define RETRIEVE_CHECKER_HPP
#include "engine/item.h"
/*
 *
 * I know that this is a fairly complex template setup, and would be simplified by having
 * two distinct children for the native type converison case and the item* case.
 *
 * However, that would spread the complexity into the rest of the program, where the
 * user would have to specify whether a type was being checked with the native checking
 * API or the item* API.
 *
 * I am choosing in this case (and doing my best to with the other specialized template types)
 * to consolidate complexity into one location and allow the user to take advantage of generic programming
 * without ealing with the in's and out's of it.
 *
 * Maybe if a newer C++ version allows for template specialization inside of a template class this would be
 * simpler. Also, there's probably a better way and I just don't know template metaprogramming that well
 * 
 * If I have to use dynamic cast like this again I'm just going to write a new version of dynamic cast so it
 * just returns 0 and doesn't fail on compilation.
 *
 */

namespace __HIDER__{
    //helper classes to help with template specialization
    template<class T, bool>
        struct __retrieve_helper{
            public:
                //This cannot be a compile time since these functions are always instantiated 
                //regardless of the true/false, even for types that don't call it
                static void check(const item* inval, T& val){
                    err(std::string() + "an value of type " + typeid(T).name() + 
                            " is trying to retrieve a value from " +
                            inval->name() + ", an item of incompatible type " + inval->type(),
                            "__retrieve_checker::check_and_get_type(item*)", 
                            "engine/item.h", FATAL_ERROR);
                }
        };
    template<class T>
        struct __retrieve_helper<T, true>{
            //inside this struct we can treat T as a pointer type
            //since this can only get here it T is a pointer inside the item heirarchy
            public:
                static void check(item* inval, T& val){
                    //if only dynamic cast just returned 0 when it statically couldn't cast
                    //If I find the need to repeat this idiom again I'll write out another dynamic_cast
                    //struct that would just return 0 upon a failed static cast, like int* to item*
                    T i_cast = dynamic_cast<T>(inval);
                    if(!i_cast){
                        err(std::string() + "An item of type " + val->type() + 
                                " is trying to retrieve a value from " +
                                inval->name() + ", an item of incompatible type " + inval->type(),
                                "__retrieve_checker::check_and_get_type(item*)", 
                                "engine/item.h", FATAL_ERROR);
                    }
                    val = i_cast;
                }
        };
}


//!Pointer wrapper to ensure type-safety in the retrieve function, while removing the need for a template in the rest of the code
class retrieve_wrapper{
    public:
        //Although void* is the opposite of type safety, it is simply an unparameterized wrapper
        //to a type-safe function
        virtual void check_and_get_type(const std::type_info& type, void* inval) = 0;
        virtual void check_and_get_type(item* inval) = 0;
        virtual void* get_ptr() = 0;
        virtual ~retrieve_wrapper(){}
};

//Forwards declarations for friend functions
template<class T> class __retrieve_checker;
template<class T> void retrieve(T& inval, item* setter, item* getter);
template<class T> void retrieve(T& inval, std::shared_ptr<item> setter, item* getter);

//main retrieve_checker class
template<class T> class __retrieve_checker:public retrieve_wrapper{
    T& val;
    __retrieve_checker(T& inval):val(inval){
    }
    public:
    virtual ~__retrieve_checker(){}
    //this is to assist with data structures dealing with pointers
    virtual void* get_ptr(){
        return &val;
    }
    virtual void check_and_get_type(const std::type_info& type, void* inval){
        if(typeid(T) == type){
            val = *(T*)inval;
            return;
        }
        err(std::string() + "An value of type " + typeid(T).name() + 
                " is trying to retrieve a value from a type of " +
                type.name(),
                "__retrieve_checker::check_and_get_type(item*)", 
                "engine/item.h", FATAL_ERROR);
    }

    public:
    inline void check_and_get_type(item* inval){
        //The type traits ensure that first, T is a pointer (hopefully an item *),
        //and second, that whatever type T is a pointer to is part of the item hierarchy
        constexpr bool is_item_ptr = std::is_convertible<T, item*>::value;
        __HIDER__::__retrieve_helper<T, is_item_ptr>::check(inval, val);
    }
    friend void retrieve<T>(T& inval, item* setter, item* caller);
};


//This function removes the need to call item->retrieve AND retrieve checker while streamling standard values,
//allowing for cleaner code. It also may help to abstract this operation,
//should it change. Finally, it blocks access to the retrieve function and the
//retrieve_checker class;
template<class T> inline void retrieve(T& inval, item* sender, item* caller){
    if(sender){
    sender->_retrieve(__retrieve_checker<T>(inval), caller);
    }
    else{
        if(caller){
            err(caller->name() + "Has requested an item that does not exist", "retrieve",
                    "utils/retrieve_checker.hpp", caller, FATAL_ERROR);
        }
        else{
            err("A nonexistent item has been requested", "retreive", "utils/retrieve_checker.hpp",
                    FATAL_ERROR);
        }
    }
}

template<class T> inline void retrieve(T& inval, std::shared_ptr<item> sender, item* caller){
    if(sender.use_count()){
        retrieve(inval, sender.get(), caller);
    }
    else{
        if(caller){
            err(caller->name() + "Has requested an item that does not exist", "retrieve",
                    "utils/retrieve_checker.hpp", caller, FATAL_ERROR);
        }
        else{
            err("A nonexistent item has been requested", "retreive", "utils/retrieve_checker.hpp",
                    FATAL_ERROR);
        }
    }
}

//This allows for an optional value to be passed, and simply creates that 
template<class T> inline void retrieve(T& inval, item* sender, item* caller, const T& standard){
    if(sender){
        retrieve(inval, sender, caller);
    }
    else{
        inval = standard;
    }
}

template<class T> inline void retrieve(T& inval, std::shared_ptr<item> sender, item* caller, const T& standard){
    if(sender.use_count()){
        retrieve(inval, sender.get(), caller);
    }
    else{
        inval = standard;
    }
}

#endif
