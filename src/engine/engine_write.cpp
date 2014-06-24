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
    //this will be slightly innaccurate, but is easier than trying to figure out how many characters
    //the stream will use to represent ind
    size_t dat_size = sizeof("index: ") + sizeof("\n") + (size_t)std::floor(std::log10(ind));
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
//!Writes formatted data to disk and clears from ram
void engineimp::write_dat(){
    constexpr size_t max_size = 1e9; //won't add more data while ~1gb of data is waiting to be held
    while(data_size > max_size){
        //nothing is writing at 2gb a second, this might clear a few hundred mb if we are lucky!!!
        std::chrono::milliseconds sleep_len(500);
        std::this_thread::sleep_for(sleep_len);
    }
    auto dat = std::make_shared<data_blob>();
    dat->size = ::write_dat(writers,dat->dat);

    std::unique_lock<std::mutex> dat_lock(data_lock);
    datas_queued++;
    data_size += dat->size;
    async_write_queue.push_back(dat);
    dat_lock.unlock();

    writers.clear();
    std::unique_lock<std::mutex> lock(condition_lock);
    write_cond.notify_all();
    lock.unlock();
}

