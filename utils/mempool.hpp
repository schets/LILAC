#pragma once
#ifndef MEMPOOL_HPP
#define MEMPOOL_HPP
#include "defs.hpp"
#include <list>
#include <stdint.h>
class mempool;
namespace __HIDER__{
    template <typename... Args>
        struct Impl;

    template <typename Th, typename... Tl>
        struct Impl<Th, Tl...>
        {
            static std::list<size_t> _create()
            {
                auto tmp = Impl<Tl...>::_create();
                tmp.push_front(sizeof(Th));
                return tmp;
            }
        };

    template <>
        struct Impl<>
        {
            static std::list<size_t>  _create()
            {
                return std::list<size_t>();
            }
        };

        struct ptr_make
        {
            
        };
        template <typename... Args>
            struct lmake;

        template <typename Th, typename... Tl>
            struct lmake<Th, Tl...>
            {
                static void _create(std::list<void**>& ival, Th* restr * hval, Tl* restr *... lvals)
                {
                    ival.push_back((void**)hval);
                    lmake<Tl...>::_create(ival, lvals...);
                }
            };

        template <>
            struct lmake<>
            {
                static void  _create(std::list<void**>& ival){}
            };

}
//!This provides a class to assisst with memory management.
/*!
 * This class helps with allocating memory for use in computations.
 * It creates a list of pointers that are all aligned to a specified
 * value and are near each other in memory
 */

class mempool{
    static void _create(size_t al, char* v, char* en, 
            std::list<size_t> sizes, std::list<size_t> vsizes, std::list<char* >& ptrl);
    /*! Charizard is the third evolution of flame pokemon charmander. Very powerful and difficult to tame.
     * Also known for it's ability to store the raw memory used in memory pools
     */
    char* izard;//charizard lol.
    size_t total_bytes;
    std::list<size_t> csizes, vsizes;
    std::list<void**> cptrs; 
    template<class ...Tl>
        void make_this(size_t mal, size_t dim, Tl* restr *... args);
    template<class ...Tl>
        void make_this(size_t dim, Tl* restr *... args){
            make_this(32, dim, args...);
        }

    public:
    template<class ...Tl>
        void create(size_t dim, Tl* restr *... args){
            clear();
            make_this(32, dim, args...);
        }
    template<class ...Tl>
        void create(size_t mal, size_t dim, Tl* restr *... args){
            clear();
            make_this(mal, dim, args...);
        }
    template<class ...Tl>
        void add(size_t mal, size_t dim, Tl* restr * ... args);
    template<class ...Tl>
        void add(size_t dim, Tl* restr * ... args){
            add(32, dim, args...);
        }
    void set_dim(size_t num, size_t mal=32);
    void clear();
    mempool();
    ~mempool();
};


template<class ...Tl>
void mempool::make_this(size_t mal, size_t dim, Tl* restr *... args){
    csizes.clear();
    cptrs.clear();
    auto val = __HIDER__::Impl<Tl...>::_create();
    //for now, just allocate (n+1)*n_align + size_total
    total_bytes = (val.size() +1)*mal;
    std::list<char*> held_ptrs;
    for(auto s : val){
        csizes.push_back(dim);
        vsizes.push_back(s);
        total_bytes += dim * s;
    }
    bool is_done=false;
    while(!is_done){
        is_done=true;
        izard= (char*)al_malloc(total_bytes);
        try{
            _create(mal, izard, izard+total_bytes,
                    csizes, vsizes, held_ptrs);
        }
        catch(std::exception& e){
            std::cout << "Needs more memory" << std::endl;
            is_done=false;
            al_free(izard);
            total_bytes *= 2;
        }
    }
    __HIDER__::lmake<Tl...>::_create(cptrs, args...);
    auto h = held_ptrs.begin();
    auto c = cptrs.begin();
    for(; h != held_ptrs.end() && c != cptrs.end();){
        *(*c) = *h;
        h++;
        c++;
    }
}

template<class ...Tl>
void mempool::add(size_t mal, size_t dim, Tl* restr * ... args){
    auto val = __HIDER__::Impl<Tl...>::_create();
    //for now, just allocate (n+1)*n_align + size_total
    size_t new_bytes = (val.size() +1)*mal;
    std::list<char*> held_ptrs;
    for(auto s : val){
        csizes.push_back(dim);
        vsizes.push_back(s);
        new_bytes += dim * s;
    }
    total_bytes+=new_bytes;
    bool is_done=false;
    if(izard){
        al_free(izard);
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
    __HIDER__::lmake<Tl...>::_create(cptrs, args...);
    auto h = held_ptrs.begin();
    auto c = cptrs.begin();
    for(; h != held_ptrs.end() && c != cptrs.end();){
        *(*c) = *h;
        h++;
        c++;
    }
}
#endif
