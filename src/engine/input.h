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
#ifndef INPUT_H
#define INPUT_H
#include "item.h"
#include "item_wrapper.h"
#include "utils/retrieve_checker.hpp"
//!This class wraps the raw input for retrieval purposes
/*
 * The input class serves to abstract the actual values that were created by the input
 * away from the act of retrieving them. This is used for two purposes as of now:
 *      - When a variable is defined locally in another varaible, this class provides the remapping
 *      - When a variable name is mapped to another, this class performs the mapping
 *
 *  In addition, this class prevents other classes from ever seeing the actual input representation
 *
 *  Inside the engine, each variable is given an input class, which serves to make redirections and mappings local
 */
class input{
    //!Points to the actual map of input values
    std::map<std::string, item_wrapper>* invals;

    //!Re-directs an input value to a locally defined item
    std::map<std::string, item_wrapper> re_directions;

    //!Re-directs one name to another
    std::map<std::string, std::string> re_mappings;

    //!gets a value from the input
    item_wrapper& get_val(const std::string& value);
    public:
    input(){}
    input(std::map<std::string, item_wrapper>& values);
    inline const item* operator[](const std::string& inval){
        return get_val(inval).get();
    }

    //!Redirects an input value to a local item
    void add_redir(const std::string& name, const item_wrapper& inv);

    //!Remaps one name to another item. Mappings will prioritize local variables over global
    void add_remap(const std::string& oldname, const std::string& newname);

    void insert_item(std::shared_ptr<item> inval);
    //templated version to simplfy passing base classes
    template<class T>
        void insert_item(std::shared_ptr<T> inval){
            static_assert(std::is_base_of<item, T>::value,
                    "Must insert types which are a base of item into the input");
            insert_item(std::dynamic_pointer_cast<item>(inval));
        }
    friend void eval_lisp(std::string in_cmd, input& inv);



    //!Type safe method of retrieving value from a variable
    /*!
     * This function provides a type-safe interface for extracting values from the item class
     * The variable type is automatically inferred from the passed type-
     * For example, passing an int to recieve an unsigned will result in a failure, as will passing anything other than a size_t.
     * Same goes for doubles and floats
     *
     * When retrieving item* type classes, the type of pointer passed must be in the same inheritance line as the 
     * value being retrieved.
     *
     * @param inval A reference to the variable that will recieve the internal value of the item
     * @param name The name of the item being retrieved
     * @param caller The item receiving the variable
     */
    template<class T>
        void retrieve(T& inval, const std::string& name, item* caller){

            item* sender = get_val(name).get();

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
//!Retrieve that allows passing of a constructor function
    /*!
     * This retrieve function allows the passing of a function type argument to construct
     * the value if the item does not exist in the input.
     * It must be an actual function type object, not a callable object or a function pointer.
     * See the C++ reference on std::is_function for a more detailed overview on what constitutes a function
     */
    template<class T, class Lambda>
        typename std::enable_if<!std::is_convertible<T, Lambda>::value>::type
        retrieve(T& inval, const std::string name, item* caller, Lambda&& ctor){
            item* sender = get_val(name).get();
            if(sender){
                sender->_retrieve(__retrieve_checker<T>(inval), caller);
            }
            else{
                inval=std::forward<Lambda>(ctor)();
            }
        }
    /*!
     * Adds an optional value that will be placed in inval if the item does not exist
     * The type passed to the function must be the convertible to the type being retrieved
     * 
     * @param stdval The value that inval is set to if the passed item does not exist
     */
    template<class T, class U>
        typename std::enable_if<std::is_convertible<T, U>::value>::type
        retrieve(T& inval, const std::string& name, item* caller, U&& stdval){
            item* sender = get_val(name).get();
            if(sender){
                sender->_retrieve(__retrieve_checker<T>(inval), caller);
            }
            else{
                inval=std::forward<T>(stdval); 
            }
        }
    template<class T, class U>
        typename std::enable_if<std::is_same<T, U>::value>::type
        retrieve(T& inval, const std::string& name, item* caller, const U& stdval){
            item* sender = get_val(name).get();
            if(sender){
                sender->_retrieve(__retrieve_checker<T>(inval), caller);
            }
            else{
                inval=stdval;
            }
        }

    


    //non-tracking versions of retrieve
    /*!
     * Retrieves a value without tracking the caller. This is not reccomended
     * since it prevents variable changes from being broadcasted to the caller
     * \sa input::retrieve
     */

    template<class T>
        void retrieve_notrack(T& inval, const std::string& name){
            retrieve(inval, name, 0);
        }
    /*!
     * Non-tracking retrieve with standard value or creation function
     * \sa input::retrieve
     */
    template<class T, class U>
        void retrieve_notrack(T& inval, const std::string& name, U&& stdval){
            retrieve(inval, name, 0, stdval);
        }

    /*!
     * Non-tracking retrieve with standard value
     * \sa input::retrieve
     */
    template<class T, class U>
        void retrieve_notrack(T& inval, const std::string& name, const U& stdval){
            retrieve(inval, name, 0, stdval);
        }
};
#endif
