#include "engine.h"
#include "engineimp.h"
#include "item.h"
#include "defs.h"
#include <stdlib.h>
char f_is_empty(std::ifstream& fstr){
    return fstr.peek()==std::ifstream::traits_type::eof();
}
engineimp::engineimp(const std::string fname, const std::string outname, const std::string index){
    values["!out_file"] = new string();
    values["!out_file"]->parse(outname);
    values["!out_file"]->setname("!out_file");
    integer* p = new integer();
    p->parse(index);
    p->setname("!start_ind");
    values["!start_ind"]=p;
    std::ifstream fstr(fname.c_str());
    if(f_is_empty(fstr)){
        err("Empty/non-existant file passed as configuration parameter",
                "engineimp::engineimp(std::string)", "parser/engine.cpp", FATAL_ERROR);
    }
    read(fstr);
};

engineimp::~engineimp(){
    std::map<std::string, item*>::iterator beg;
    for(beg=values.begin(); beg != values.end(); beg++){
        if(beg->second){
            //      std::cout << "Starting deletion for " << beg->second->name() << std::endl;
            delete beg->second;
        }
    }
}

char engineimp::item_exists(item* p) const{
    if(!p){
        return 1;
    }
    std::map<std::string, item*>::const_iterator mbeg;
    for(mbeg = values.begin(); mbeg != values.end(); mbeg++){
        if(p == mbeg->second){
            return 1;
        }
    }
    return 0;
}
/*!
 * Updates items which reference variables that require changing,
 * and resets the list of items which require updating
 */
void engineimp::update(){
    std::set<item*>::iterator beg;
    for(beg = update_vars.begin(); beg != update_vars.end(); beg++){
        (*beg)->update();
    }
    update_vars.clear();
}
/*!
 * Adds an item to the list of items that require updating
 * @param name Name of the item that requires updating
 */
void engineimp::needs_updating(std::string name){
    std::map<std::string, item*>::iterator pos = values.find(name);
    if(pos == values.end()){
        std::string errmess = "Lookup for item \"";
        errmess.append(name);
        errmess.append("\" has failed");
        err(errmess, "engineimp::needs_updating", "parser/engine.cpp", WARNING);
        return;
    }
    needs_updating(pos->second);
}
void engineimp::needs_updating(item* inval){
    if(!inval){
        err("Null pointer passed to needs_updating", "engineimp::needs_updating",
                "parser/engine.cpp", WARNING);
        return;
    }
    update_vars.insert(inval);
}
/*
 *
 *Engine wrapper functions
 *
 *
 *
 *
 */

/*!
 * Constructs an engine object, a wrapper for engimeimp
 * \sa engineimp
 */
engine::engine(const std::string fname, const std::string outname, std::string index){
    eng = new engineimp(fname, outname, index);
}
engine::~engine(){
    if(eng){
        delete eng;
    }
}
void engine::run(){
    eng->run();
}
