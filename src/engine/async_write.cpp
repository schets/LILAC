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
static void write_data(std::shared_ptr<writer> dat, std::ostream& wfile){
    dat->write(wfile);
    wfile << "&&\n";
}
static void write_individual_dat(const std::list<std::shared_ptr<writer>>& dats, size_t ind, std::ostream& out_stream){
    out_stream << "index: " << ind << "\n";
    for(auto& dat : dats){
        write_data(dat, out_stream);
    }
    out_stream << "&\n";
}
static size_t write_dat(std::map<size_t, std::list<std::shared_ptr<writer>>>& writers, FILE* ofile){
    std::stringstream out_stream;
    for(auto& writes : writers){
        write_individual_dat(writes.second, writes.first, out_stream);
    }
    const auto& o_str = out_stream.str();
    fwrite(o_str.c_str(), sizeof(char), o_str.size(), ofile);
    writers.clear();
    return o_str.size();
}
//This is somewhat of a mess of mutexes, condition variables, and atomic conditions.
//I'm this can be cleaned up but it allows the engine to add data for writing while data is being currently written
//and the thread will never exit until the engine ends.
//!worker function for asynchronous data io
void write_data(std::mutex& wait_m, std::mutex& data,
        std::condition_variable& write_notify, data_io_info invals){
    FILE* ofile = invals.file;
    auto* writers = invals.writers;
    volatile std::atomic_size_t* datas_in_queue = invals.datas_queued;
    volatile std::atomic_char* is_over = invals.is_over;
    while(true){
        //obtain a lock on the data structure
        std::unique_lock<std::mutex> data_lock(data);
        //wait until more data exists to write
        while(writers->empty()){
            //unblock data access
            data_lock.unlock();
            //if waiting for data and engine ends, just quit
            if(*is_over == 1){
                return;
            }
            //no data available, wait for next data run
            std::unique_lock<std::mutex> lock(wait_m);
            write_notify.wait(lock, [writers](){return !writers->empty();});
            //relinquish access to notifier lock
            lock.unlock();
            //re-obtain data access
            data_lock.lock();
        }
        if(*is_over == 1 && writers->empty()){
            return;
        }
        //move smeantics since this value is being destroyed anyways
        auto val_from_front = std::move(writers->front());
        writers->pop_front();
        //relinquish access to data, it isn't needed anymore
        data_lock.unlock();
        //perform data io and subtract total from 
        write_dat(val_from_front, ofile);
        (*datas_in_queue)--;
        std::cout << (size_t)*datas_in_queue << std::endl;
    }
}
