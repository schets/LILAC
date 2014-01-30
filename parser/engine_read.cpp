#include "engineimp.h"
#include <string>
#include <map>
#include <list>
#include "graph.h"
#include "item.h"
#include "comp_funcs.h"
void engineimp::read(std::ifstream& fstr){
    const std::string delim=" ";
    const std::string comment="#";
    std::string token;
    std::string curline;
    std::list<item*> post_order;
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
        post_order.push_back(curit);
        values[token]=curit; 
        //get the current line from the string
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
};



