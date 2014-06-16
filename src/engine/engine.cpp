#include "engine.h"
#include "engineimp.h"
#include "item.h"
#include "input.h"
#include "native_item.hpp"
char f_is_empty(std::ifstream& fstr){
    return fstr.peek()==std::ifstream::traits_type::eof();
}
engineimp::engineimp(const std::string& fname, const std::string& outname, const std::string& index){
    values["!out_file"] =  std::make_shared<string>();
    ((native_item*)(values["!out_file"].get()))->parse(outname);
    values["!out_file"]->setname("!out_file");
    ofile.open(outname.c_str(), std::ofstream::trunc);
    auto p = std::make_shared<integer>();
    p->parse(index);
    p->setname("!start_ind");
    values["!start_ind"]=p;
    std::ifstream fstr(fname.c_str());
    if(f_is_empty(fstr)){
        err("Empty/non-existant file passed as configuration parameter",
                "engineimp::engineimp(std::string)", "engine/engine.cpp", FATAL_ERROR);
    }
    read(fstr);
};

engineimp::~engineimp(){
    this->write_dat();
}
bool engineimp::item_exists(std::shared_ptr<item> p) const{
    if(!p){
        return false;
    }
    for(auto& val : values){
        if(val.second.get_shared() == p){
            return true;
        }
    }
    return false;
}
bool engineimp::item_exists(const std::string& val) const{
    return values.count(val);
}
/*!
 * Updates items which reference variables that require changing,
 * and resets the list of items which require updating
 */
void engineimp::update(){
    for(auto& val : update_vars){
        val->update();
    }
    update_vars.clear();
}
/*!
 * Adds an item to the list of items that require updating
 * @param name Name of the item that requires updating
 */
void engineimp::needs_updating(std::string name){
    auto pos = values.find(name);
    if(pos == values.end()){
        std::string errmess = "Lookup for item \"";
        errmess.append(name);
        errmess.append("\" has failed");
        err(errmess, "engineimp::needs_updating", "engine/engine.cpp", WARNING);
        return;
    }
    needs_updating(pos->second);
}
/*!
 * Adds an item to the list of items that require updating
 * Goes by pointer so that items that are not part of the engine
 * can be updated. Useful for wrapper classes such as the integrators
 * @param inval Pointer to the item that requires updating
 */
void engineimp::needs_updating(std::shared_ptr<item> inval){
    if(!inval){
        err("Null pointer passed to needs_updating", "engineimp::needs_updating",
                "engine/engine.cpp", WARNING);
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
