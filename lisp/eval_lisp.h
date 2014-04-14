#pragma once
#ifndef EVAL_LISP_H
#define EVAL_LISP_H
#include <string>
#include <list>
#include <map>
#include <memory>
class input;
class engineimp;
class item_wrapper;
struct param_val{
    std::string type;
    std::string name;
    std::string params;
};

std::list<std::string> split_into_sepxr(std::string in_expr);

item_wrapper eval_lisp(std::string in_cmd, std::string base_name, std::map<std::string, input>& inv, engineimp* en);

param_val split_into_toks(std::string param_vals, bool has_paren=true);
#endif
