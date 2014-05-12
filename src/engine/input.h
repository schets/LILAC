#pragma once
#ifndef INPUT_H
#define INPUT_H
#include "item.h"
#include "item_wrapper.h"
class input;
void eval_lisp(std::string in_cmd, input& inv);
class input{
    std::map<std::string, item_wrapper>* invals;
    std::map<std::string, item_wrapper> re_directions;
    std::map<std::string, std::string> re_mappings;
    public:
    input(){}
    input(std::map<std::string, item_wrapper>& values);
    item_wrapper& operator [](const std::string& inval);
    void add_redir(const std::string& name, const item_wrapper& inv);
    void add_remap(const std::string& oldname, const std::string& newname);
    friend void eval_lisp(std::string in_cmd, input& inv);
};
#endif