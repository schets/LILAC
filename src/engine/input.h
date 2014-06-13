#pragma once
#ifndef INPUT_H
#define INPUT_H
#include "item.h"
#include "item_wrapper.h"
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
    public:
    input(){}
    input(std::map<std::string, item_wrapper>& values);
    //!Looks up an item
    item_wrapper& operator [](const std::string& inval);
    
    //!Redirects an input value to a local item
    void add_redir(const std::string& name, const item_wrapper& inv);

    //!Remaps one name to another item. Mappings will prioritize local variables over global
    void add_remap(const std::string& oldname, const std::string& newname);
    friend void eval_lisp(std::string in_cmd, input& inv);
};
#endif
