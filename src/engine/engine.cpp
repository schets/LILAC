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
#include "engine.h"
#include "engineimp.h"
#include "item.h"
#include "input.h"
#include "native_item.hpp"


void write_data(std::mutex& wait_m, std::mutex& data,
        std::condition_variable& write_notify, data_io_info invals);

char f_is_empty(std::ifstream& fstr){
    return fstr.peek()==std::ifstream::traits_type::eof();
}
engineimp::engineimp(const std::string& fname, const std::string& outname,
        const std::string& start_index):datas_queued(0), data_size(0), is_over(0),
    cur_blob(std::make_shared<data_blob>()){
        cur_blob->size=0;
        values["!out_file"] =  std::make_shared<string>();
        ((native_item*)(values["!out_file"].get()))->parse(outname);
        values["!out_file"]->setname("!out_file");
        ofile.open(outname.c_str(), std::ofstream::out | std::ofstream::trunc);
        auto p = std::make_shared<integer>();
        p->parse(start_index);
        p->setname("!start_ind");
        values["!start_ind"]=p;
        std::ifstream fstr(fname.c_str());
        if(f_is_empty(fstr)){
            err("Empty/non-existant file passed as configuration parameter",
                    "engineimp::engineimp(std::string)", "engine/engine.cpp", FATAL_ERROR);
        }
        read(fstr);
        datas_queued=0;
        data_io_info dat_inf;
        dat_inf.data_size=&data_size;
        dat_inf.datas_queued = &datas_queued;
        dat_inf.file = &ofile;
        dat_inf.writers = &async_write_queue;
        dat_inf.is_over = &is_over;
        write_thread = std::thread(
                [dat_inf, this](){
                ::write_data(this->condition_lock, this->data_lock, this->write_cond, dat_inf);
                });
    };

engineimp::~engineimp(){
    this->_write_dat(true);
    is_over++;
    while(datas_queued > 0){
    }
    std::unique_lock<std::mutex> lock(condition_lock);
    write_cond.notify_all();
    write_thread.join();
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
