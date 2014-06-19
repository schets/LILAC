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
#pragma once
#ifndef MEMPOOL_HPP
#define MEMPOOL_HPP
#include "defs.hpp"
#include <list>
#include <stdint.h>
#include <assert.h>
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
    //!Creates memory pool with alignment of 32 bytes
    /*!
     * Creates a memory pool with an alignment of 32 bytes, and sets each pointer to its respective position.
     * @param dim the length of each pointer
     * @param args the address of each pointer
     */
    template<class ...Tl>
        void create(size_t dim, Tl* restr *... args){
            clear();
            make_this(32, dim, args...);
        }
    //!Creates memory pool with specified alignment
    /*!
     * Creates a memory pool with specified alignment, and sets each pointer to its respective position.
     * @param dim the length of each pointer
     * @param args the address of each pointer
     * @param mal The alignment in bytes
     */
    template<class ...Tl>
        void create(size_t mal, size_t dim, Tl* restr *... args){
            clear();
            make_this(mal, dim, args...);
        }
    /*!
     * Adds new pointers to the memory pool, with an alignment of 32 bytes
     */
    template<class ...Tl>
        void add(size_t mal, size_t dim, Tl* restr * ... args);
    template<class ...Tl>
        void add(size_t dim, Tl* restr * ... args){
            add(32, dim, args...);
        }
    //!Resets the dimension and alignment of the pool
    /*!
     * Sets the dimension and alignment of each pointer in the memory pool
     *
     * @param num New dimension of the memory pool
     * @param mal Alignment of the pointers
     */
    void set_dim(size_t num, size_t mal=32);
    //!Clears the memory pool and deallocates the memory
    void clear();
    mempool();
    ~mempool();
};

/*!
 * Performs the creation of the memory pool, with alignment mal, length dim,
 *  and types Tl...
 *
 *  @param mal The alignment in bytes
 *  @param dim the dimension of the pointers
 *  @param args the addresses of each pointer
 */
template<class ...Tl>
void mempool::make_this(size_t mal, size_t dim, Tl* restr *... args){
    assert(mal >= 16);
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
        izard= new char[total_bytes];
        try{
            _create(mal, izard, izard+total_bytes,
                    csizes, vsizes, held_ptrs);
        }
        catch(std::exception& e){
            std::cout << "Needs more memory" << std::endl;
            is_done=false;
            delete [] izard;
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

/*!
 * Adds new pointers to the existing memory pool, with a dimension of dim.
 * This can be different than the previous dimensions, and will not affect previously entered pointers
 *
 * \note pointers that were directly passes to the pool will still be valid, but others pointing to that memory
 * will be invalidated as the memory is likely in a different position
 *
 * @param mal The alignment in bytes
 * @param dim The dimension of each pointer
 * @param args The addresses of the pointers
 */
template<class ...Tl>
void mempool::add(size_t mal, size_t dim, Tl* restr * ... args){
    assert(mal >= 16);
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
        delete[] izard;
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
            delete [] izard;
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
