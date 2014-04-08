#ifndef MEMPOOL_HPP
#define MEMPOOL_HPP
#pragma once
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
                static void _create(std::list<char*> pval, std::list<void**>& ival, Th* restr * hval, Tl* restr *... lvals)
                {
                    ival.push_back((void**)hval);
                    lmake<Tl...>::_create(pval, ival, lvals...);
                }
            };

        template <>
            struct lmake<>
            {
                static void  _create(std::list<char*> pval, std::list<void**>& ival){}
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
            std::list<size_t> sizes, std::list<char* >& ptrl) 
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
        size_t cur_size = sizes.front();
        sizes.pop_front();
        ptrl.push_back((char*)v);
        v+= cur_size;
        _create(al, v, en, sizes, ptrl);
    }
    /*! Charizard is the third evolution of flame pokemon charmander. Very powerful and difficult to tame.
     * Also known for it's ability to store the raw memory used in memory pools
     */
    char* izard;//charizard lol.
    size_t total_bytes;
    std::list<size_t> csizes;
    std::list<void**> cptrs;
    template<size_t mal, class ...Tl>
        void make_this(size_t dim, Tl* restr *... args){
            csizes.clear();
            cptrs.clear();
            auto val = __HIDER__::Impl<Tl...>::_create();
            //for now, just allocate (n+1)*n_align + size_total
            total_bytes = (val.size() +1)*mal;
            std::list<char*> held_ptrs;
            for(auto s : val){
                csizes.push_back(dim*s);
                total_bytes += dim * s;
            }
            bool is_done=false;
            while(!is_done){
                is_done=true;
                izard= (char*)al_malloc(total_bytes);
                try{
                    _create(mal, izard, izard+total_bytes,
                            csizes, held_ptrs);
                }
                catch(std::exception& e){
                    std::cout << "Needs more memory" << std::endl;
                    is_done=false;
                    al_free(izard);
                    total_bytes *= 2;
                }
            }
            __HIDER__::lmake<Tl...>::_create(held_ptrs, cptrs, args...);
            auto h = held_ptrs.begin();
            auto c = cptrs.begin();
            for(; h != held_ptrs.end() && c != cptrs.end();){
                *(*c) = *h;
                h++;
                c++;
            }
        }
    public:
    template<size_t mal = 32, class ...Tl>
        void create(size_t dim, Tl* restr *... args){
            make_this<mal, Tl...>(dim, args...);
        }
    template<size_t mal=32, class ...Tl>
        void add(size_t dim, Tl* restr * ... args){
            size_t nbytes;
            std::list<size_t> sizes;
        }
    mempool():izard(0){}
    ~mempool(){
        if(izard){
            al_free(izard);
        }
    }
};
#endif
