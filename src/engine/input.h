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

    /*!
     * Adds an optional value that will be placed in inval if the item does not exist
     * 
     * @param stdval The value that inval is set to if the passed item does not exist
     */
    template<class T>
        void retrieve(T& inval, const std::string& name, item* caller, T&& stdval){
            item* sender = get_val(name).get();
            if(sender){
                sender->_retrieve(__retrieve_checker<T>(inval), caller);
            }
            else{
                inval=std::forward<T>(stdval); 
            }
        }
    template<class T>
        void retrieve(T& inval, const std::string& name, item* caller, const T& stdval){
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
     * Non-tracking retrieve with standard value
     * \sa input::retrieve
     */
    template<class T>
        void retrieve_notrack(T& inval, const std::string& name, T&& stdval){
            retrieve(inval, name, 0, stdval);
        }

    /*!
     * Non-tracking retrieve with standard value
     * \sa input::retrieve
     */
    template<class T>
        void retrieve_notrack(T& inval, const std::string& name, const T& stdval){
            retrieve(inval, name, 0, stdval);
        }

};
#endif
