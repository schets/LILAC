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
#include "engineimp.h"
#include "writer/writer.h"
#include "utils/item_heads.hpp"
#include <ostream>
static size_t write_data(std::shared_ptr<writer> dat, std::ostream& wfile){
    size_t dat_size = dat->write(wfile);
    wfile << "&&\n";
    return dat_size + sizeof("&&\n");
}
//only call with empty ostream, and then append that ostream onto a bigger one
static size_t write_individual_dat(const std::list<std::shared_ptr<writer>>& dats, size_t ind,
        std::ostream& out_stream){
    out_stream << "index: " << ind << "\n";
    //this may be slightly innaccurate, but is easier than trying to figure out how many characters
    //the stream will use to represent ind
    size_t dat_size=sizeof("index: ") + sizeof("\n") + 1;
    size_t ind_cpy=ind;
    while(ind_cpy > 9){
        ind_cpy/=10;
        dat_size++;
    }
    for(auto& dat : dats){
        dat_size += write_data(dat, out_stream);
    }
    out_stream << "&\n";
    //std::cout << out_stream << std::endl;
    return dat_size + sizeof("&\n");
}
static size_t write_dat(std::map<size_t, std::list<std::shared_ptr<writer>>>& writers, std::ostream& out_stream){
    size_t total_size=0;
    for(auto& writes : writers){
        total_size += write_individual_dat(writes.second, writes.first, out_stream);
    }
    writers.clear();
    return total_size;
}


//!Adds a new writer to the engine
void engineimp::add_writer(const std::shared_ptr<writer>& wval){
    writers[index].push_back(wval);
}
//OS still seems to block entire process during io...
//both threads run simultaneously, but the 
//!Writes data stored in writer to a buffer and writes to disk asynchronously every 50mb. Will block while > ~500mb of data is waiting to be written
void engineimp::_write_dat(bool force){
    //easier than dealing with orders of 1024
    constexpr size_t max_size = 5e8; //won't add more data while ~500mb of data is waiting to be held
    constexpr size_t write_size=5e7; //write every 50mb
    while(data_size > max_size - write_size){
        //nothing is writing at 2.5gb a second, this might clear a hundred mb if we are lucky!!!
        //Better to let this thread sleep instead of waste cpu cycles
        //Also, we wait on max_size - write_size since we may also have write_size worth of data in the engine
        std::chrono::milliseconds sleep_len(200);
        std::this_thread::sleep_for(sleep_len);
    }
    cur_blob->size += ::write_dat(writers, cur_blob->dat);
    if(force || cur_blob->size >= write_size){
        std::unique_lock<std::mutex> dat_lock(data_lock);
        datas_queued++;
        data_size += cur_blob->size;
        async_write_queue.push_back(cur_blob);
        dat_lock.unlock();
        cur_blob = std::make_shared<data_blob>();
        cur_blob->size=0;
        std::unique_lock<std::mutex> lock(condition_lock);
        write_cond.notify_all();
        lock.unlock();
    }
}
void engineimp::write_dat(){
    _write_dat(false);
};
