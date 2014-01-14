#include "engineimp.h"
#include <string>
#include <map>
#include <list>
#include "graph.h"
#include "item.h"
#include "creator.h"
void trim(std::string& curline){
    size_t startpos = curline.find_first_not_of("\n\r\t ");
    if(startpos != std::string::npos){
        curline=curline.substr(startpos);
    }
    startpos = curline.find_last_not_of("\n\r\t ");
    if(startpos != std::string::npos){
        curline=curline.substr(0, startpos);
    }
}
void ltoken(std::string& tok, std::string& str, std::string delim=" "){
    size_t tpos = str.find(delim);
    tok=str.substr(0, tpos);
    str.erase(0, tpos + delim.length());
}
void engineimp::read(std::ifstream& fstr){
    const std::string delim=" ";
    const std::string comment="#";
    std::string token;
    std::string curline;
    std::list<item*> post_order;

    //get first line
    std::getline(fstr, curline);
    while(fstr.good()){

        //remove comments
        curline.remove(curline.find(comment, curline.end()));
        
        //trim leading and trailing whitespace
        trim(curline);

        //skip rest of loop is entire string deleted
        if(curline.empty()){
            continue;
        }
        //retrieve the first token
        token="";
        //get the current line from the string
        std::getline(fstr, curline);
    }
    graph gg;
    //topologically sort the variables
    //so that variables can only depend
    //on items previously in the list
    gg.sort(post_order);
};


