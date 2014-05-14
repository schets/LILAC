#include "engine/engineimp.h"
#include "graph.h"
#include "engine/input.h"
#include "eval_lisp.h"

//passed a string (type name params)
void engineimp::read(std::istream& fstr){
    _read(fstr);
    sort_pp();
}
void engineimp::_read(std::istream& fstr){
    const std::string delim=" ";
    const std::string comment="!";
    const char command = '#';
    std::string token;
    std::string curline;
    int line = 0;
    while(fstr.good()){
        //get current line
        std::getline(fstr, curline);
        line++;
        //remove comments
        size_t cpos = curline.find(comment);
        if(cpos != std::string::npos){
            curline.erase(cpos, std::string::npos);
        }

        //trim leading and trailing whitespace
        trim(curline);
        //skip rest of loop if entire string deleted
        if(curline.empty()){
            continue;
        }
        //if the first character is a !, then a system command is being executed
        if(curline[0] == command){
            curline.erase(0, 1);
            execute_command(curline);
            continue;
        }

        item_wrapper curit = eval_lisp(curline, std::string(), inputs, this);
        values[curit->name()] = item_wrapper(curit); 
    }
    //get the current line from the string
}

/*!
 * Sorts the items in order of dependencies and performs all of the postprocessing
 */
void engineimp::sort_pp(){
    std::list<item_wrapper> post_order;
    for(const auto& val : values){
        post_order.push_back(val.second);
    }
    graph gg;
    auto sorted(gg.sort(post_order));
    for(auto val:sorted){
        val->postprocess(inputs[val->name()]);
    }
}

void engineimp::execute_command(std::string inval){
    trim(inval);
    std::string command;
#include <list>
    ltoken(command, inval);
    if(command == "include"){
        //filename is the rest of the line
        std::string fname = curdir;
        fname.append(inval);
        std::ifstream fstr(fname.c_str());
        if(fstr.peek()==std::ifstream::traits_type::eof()){
            fname.append("Empty/non-existant file passed as configuration parameter");
            err(fname, "engineimp::engineimp(std::string)", "engine/engine.cpp", FATAL_ERROR);
        }
        _read(fstr);
    }
    else if(command=="cd"){
        curdir.append(inval);
        trim(curdir, '/');
        curdir.push_back('/');
    }
    else if(command=="setdir"){
        curdir=inval;
    }
}

