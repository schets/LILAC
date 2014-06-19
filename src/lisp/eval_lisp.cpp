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
#include "eval_lisp.h"
#include "engine/input.h"
#include "utils/defs.hpp"
#include "engine/item.h"
#include "engine/engineimp.h"
static std::set<std::string> get_keywords(){
	std::set<std::string> vals;
	vals.insert("@map");
	return vals;
}
//ICC doesn't support initializer lists yet again
static std::set<std::string> keywords = get_keywords();
param_val split_into_toks(std::string param_vals, bool has_paren){
    trim(param_vals);
    param_val p_ret;
    if(has_paren){
        param_vals.erase(param_vals.end()-1);
        param_vals.erase(param_vals.begin());
    }
    trim(param_vals);
    ltoken(p_ret.type, param_vals);
    ltoken(p_ret.name, param_vals);
    trim(param_vals);
    p_ret.params=param_vals;
    return p_ret;
}

std::list<std::string> split_into_sepxr(std::string in_expr){
    size_t slen = in_expr.size();
    size_t i = 0;
    std::list<std::string> s_exp;
    while(true){
        while(in_expr[i] != '(' && i < slen){
            i++;
        }
        if(i >= slen - 1){
            break;
        }
        size_t start_paren=i;
        i++;
        size_t n_paren=1;
        while(n_paren > 0 && i < slen){
            if(in_expr[i] == ')'){
                n_paren--;
            }
            else if(in_expr[i] == '('){
                n_paren++;
            }
            i++;
        }
        if(n_paren > 0 || i > slen){
            break;
        }
        s_exp.push_back(in_expr.substr(start_paren, i - start_paren));
    }
    return s_exp;
}

item_wrapper eval_lisp(std::string in_cmd, std::string base_name,
        std::map<std::string, input>& inv, engineimp* en){
    if(in_cmd.empty()){
        return std::shared_ptr<item>();
    }
    auto invals = split_into_toks(in_cmd);

    auto curit = item::create(invals.type, en);
    if(invals.name.empty()){
        std::cout<<"Variables need a name and a type, "<<
            "error creating sub-variable for " << base_name << std::endl;
    }
    std::string n_prefix;
    if(base_name.empty()){
        n_prefix="";
    }
    else{
        n_prefix=base_name+"::";
    }
    //get next token, should be variable name
    curit->setname(n_prefix + invals.name);
    inv[curit->name()] = input(en->values);
    input& inval = inv[curit->name()];
    item_wrapper c_wrap(curit); 
    auto native_ptr = std::dynamic_pointer_cast<native_item>(curit);
    if(native_ptr.use_count()){
        //if token is one of the native types, parse the input
        native_ptr->parse(invals.params);
    }
    else{
        //recursively evaluate whatever expression is passed on
        for(auto& str : split_into_sepxr(invals.params)){
            //this will check if special commands are being done
            auto exp_tok=split_into_toks(str);
            //should replace this with callbacks in the map
            if(keywords.count(exp_tok.type)){
                //remap the passed variable names to single name
                if(exp_tok.type == "@map"){
                    std::string tok;
                    trim(exp_tok.params);
                    while(!exp_tok.params.empty()){
                        ltoken(tok, exp_tok.params);
                        inval.add_remap(exp_tok.name, tok);
                        c_wrap.replace_dep(exp_tok.name, tok);
                    }
                }
            }
            else{
                item_wrapper child_item = eval_lisp(str, n_prefix+invals.name, inv, en);
                if(child_item.get_shared().use_count()){
                    //replace named dependencies with new fake names
                    param_val pp = split_into_toks(str);
                    c_wrap.replace_dep(child_item->name(), pp.name);
                    inval.add_redir(pp.name, child_item);
                    en->values[child_item->name()] = child_item;
                }
            }
        }
    }
    return c_wrap;
}
