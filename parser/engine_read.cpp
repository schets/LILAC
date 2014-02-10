#include "engineimp.h"
#include <string>
#include <map>
#include <list>
#include "graph.h"
#include "item.h"
#include "comp_funcs.h"
void engineimp::read(std::ifstream& fstr){
    _read(fstr);
    sort_pp();
}
void engineimp::_read(std::ifstream& fstr){
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
        item* curit;
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
        //if a ! is not the first character, then bad input
        if(curline[0] == command){
            curline.erase(0, 1);
            execute_command(curline);
            continue;
        }
        //retrieve the first token
        ltoken(token, curline);
        //this token should be the variable type
        curit = item::create(token, this);
        if(curline.empty()){
            std::cout<<"Variables need a name and a value, "<<
                "error on line " << line << std::endl;
            continue;
        }
        //get next token, should be variable name
        ltoken(token, curline);
        curit->setname(token);
        curit->parse(curline);
        values[token]=curit; 
        //get the current line from the string
    }
};

/*!
 * Sorts the items in order of dependencies and performs all of the postprocessing
 */
void engineimp::sort_pp(){
    std::list<item*> post_order;
    std::map<std::string, item*>::iterator beg;
    for(beg = values.begin(); beg != values.end(); beg++){
        post_order.push_back(beg->second);
    }
    graph gg;
    //topologically sort the variables
    //so that variables can only depend
    //on items previously in the list
    gg.sort(post_order);

    std::list<item*>::iterator lbeg;
    for(lbeg=post_order.begin(); lbeg != post_order.end(); lbeg++){
        (*lbeg)->postprocess(values);
    }
}

void engineimp::execute_command(std::string inval){
    trim(inval);
    std::string command;
    ltoken(command, inval);
    if(command == "include"){
        //filename is the rest of the line
        std::string fname = curdir;
        fname.append(inval);
        std::ifstream fstr(fname.c_str());
        if(fstr.peek()==std::ifstream::traits_type::eof()){
            fname.append("Empty/non-existant file passed as configuration parameter");
            err(fname, "engineimp::engineimp(std::string)", "parser/engine.cpp", FATAL_ERROR);
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
