#include "mempool.hpp"
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
        al_free(izard);
        izard = 0;
    }
    while(!is_done){
        //try setting aligned pointers in currently allocated memory
        is_done=true;
        izard= (char*)al_malloc(total_bytes);
        try{
            _create(mal, izard, izard+total_bytes,
                    csizes, vsizes, held_ptrs);
        }
        catch(std::exception& e){
            is_done=false;
            al_free(izard);
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
        al_free(izard);
        izard = 0;
    }
    csizes.clear();
    cptrs.clear();
}



mempool::mempool():izard(0){}

mempool::~mempool(){
    if(izard){
        al_free(izard);
    }
}
