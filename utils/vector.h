/*
#ifndef VECTOR_H
#define VECTOR_H
#include "defs.h"
#include <iostream>
#include <string>
extern "C"{
#include <stdlib.h>
}

#ifdef MEM_CHECK
#define memcheck(n) _memcheck(n)
#define sizecheck(n) _sizecheck(n)
#else
#define memcheck(n)
#define sizecheck(n)
#endif


#ifdef EXIT_ON_VECTOR_ERROR
#define EXIT_ON_ERROR exit(1)
#else
#define EXIT_ON_ERROR exit(1);
#endif



//class to emulate matlab vector operations
//IS NOT MANAGED NEVER SHOULD BE
//is simply a wrapper to a pointer, adding matlab-like array operations
template<typename T> class vector{
    T* vals;
    size_t num;
    inline void _memcheck(size_t n, std::string name, std::string func){
        if(n < 0 || n >= num){
            std::cout << " bad memory access, "<<name<<"="<<n<<", num="<<num
                <<" in function " << func<<std::endl;
            EXIT_ON_ERROR;
        }
    }
    inline void _sizecheck(size_t n, std::string oper){
        if(num != n){
            cout << "vector "<<oper<<" of two vectors of different size\n <<
                "size="<<num<<", v.size="<<v.num<<endl;
        }
        EXIT_ON_ERROR;
    }
    public:
    //make possible effecient indexing to end/beginning
    //and other possible operations
    class beg{
    };
    class end{
    };
    class cpy{
    };
    vector(const vector<T>& v):vals(v.vals), num(v.num){};
    vector():vals(0), num(0){};
    vector(size_t n):vals(n*sizeof(T)), num(n){};
    vector(size_t n, T* ptr):num(n), vals(ptr){}
    inline const vector<T>& operator=(const vector<T>& v){
    }
    inline T& operator[](size_t ind){
        memcheck(n, ind, "operator []");
        return vals[ind];
    }
    inline const T& operator [](size_t ind) const{
        memcheck(n, ind, "operator []");
        return vals[ind];
    }
    inline vector<T> operator()(size_t o1, size_t o2){
        memcheck(o2, "o2", "operator()(size_t, size_t)");
        memcheck(o1, "o1", "operator()(size_t, size_t)");
        return vector<T>(o2-o1, vals+o1);
    }
    ~vector(){
        if(vals){
            free(vals);
        }
    }
};
#undef memcheck
#undef EXIT_ON_ERROR
#endif
*/
