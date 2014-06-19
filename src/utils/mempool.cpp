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
#include "mempool.hpp"
#include "comp_funcs.hpp"
void mempool::set_dim(size_t num, size_t mal){
    total_bytes = 0;
    for(auto& v : csizes){
        v = num;
    }
    for(auto v : vsizes){
        total_bytes += v*num;
    }
    total_bytes += (1+vsizes.size())*mal;
    std::list<char*> held_ptrs;
    bool is_done = false;
    if(izard){
        delete[] izard;
        izard = 0;
    }
    while(!is_done){
        //try setting aligned pointers in currently allocated memory
        is_done=true;
        izard= new char[total_bytes];
        try{
            _create(mal, izard, izard+total_bytes,
                    csizes, vsizes, held_ptrs);
        }
        catch(std::exception& e){
            is_done=false;
            delete[] izard;
            total_bytes *= 2;
        }
    }
    auto h = held_ptrs.begin();
    auto c = cptrs.begin();
    for(; h != held_ptrs.end() && c != cptrs.end();){
        *(*c) = *h;
        h++;
        c++;
    }
}

void mempool::_create(size_t al, char* v, char* en, 
            std::list<size_t> sizes, std::list<size_t> vsizes, std::list<char* >& ptrl) 
    {
        if(sizes.empty()){
            return;
        }
        if(v >= en){
            throw std::bad_alloc();
        }
        while(uintptr_t(v) % al){
            v++;
            if(v >= en){
                throw std::bad_alloc();
            }
        }
        size_t cur_size = sizes.front()*vsizes.front();
        sizes.pop_front();
        vsizes.pop_front();
        ptrl.push_back((char*)v);
        v+= cur_size;
        _create(al, v, en, sizes, vsizes, ptrl);
    }


void mempool::clear(){
    if(izard){
        delete [] izard;
        izard = 0;
    }
    csizes.clear();
    cptrs.clear();
}



mempool::mempool():izard(0){}

mempool::~mempool(){
    if(izard){
        delete [] izard;
    }
}
