#ifndef DEF_H
#define DEF_H
//#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <string>
#include <sstream>
#include <eigen3/Eigen/Eigen>
#include <memory>
extern "C" {
#include <complex.h>
#include <fftw3.h>
#include <math.h>
#include <time.h>
}
#ifdef ICC
#define restr restrict
#define ALIGNED(x) __assume_aligned(x, 16)
#define MAKE_ALIGNED __declspec(align(16))
#define VEC_ALIGN #pragma vector aligned
#else
#define restr __restrict__
#define ALIGNED
#define MAKE_ALIGNED
#define VEC_ALIGN
#endif
//#define restr
//typedef complex<double> comp;
typedef fftw_complex comp;
//#define NUM_TIME_STEPS (256) //number of time points
//#define LENGTH_T (60) //length of t-domain
//parameters taken from matlab main file
//can move to input parameters as well
//but for now these are defines
#define D (-.4)
#define K (.1)
#define A (2.0/3.0)
#define B (1.0/3.0) //need to find a real value for this
#define tau (.1)
#define Gamma (.1)
#define RTlength (1.5)
#define PI (3.14159)
#define maxTrips 50
#include "parser/item.h"

class _fatal{
    public:
    _fatal(){};
};
class _warning{
    public:
    _warning(){};
};
const static _fatal FATAL_ERROR;
const static _warning WARNING;
inline void err(std::string message, std::string function, std::string file, _fatal f){
    std::cout<<"A fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\nExiting program\n";
    exit(1);
}
inline void err(std::string message, std::string function, std::string file, _warning w){
    std::cout<<"A non-fatal error has occurred in function "<<function<<" in file "<<file<<
#include <cstdlib>
        "\nError message is:\n"<<message<<"\nContinuing program\n";
}
inline void err(std::string message, std::string function, std::string file, item* p, _fatal f){
    std::cout<<"A fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cout << "\nExiting program\n";
    exit(1);
}
inline void err(std::string message, std::string function, std::string file, item* p, _warning f){
    std::cout<<"A non-fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cout << "\nContinuing program\n";
}


//Alligned malloc
//also ensures return value is valid
inline void* al_malloc(size_t ss){
    void* rv = fftw_malloc(ss);
    if(!rv){
        std::stringstream str;
        str << "Memory allocation of " << ss << " bytes of data failed";
        err(str.str(), "malloc", "defs.h", FATAL_ERROR);
    }
    return rv;
}
inline void al_free(void* ss){
    if(!ss){
        err("Attempt to free a null pointer", "free", "defs.h", WARNING);
    }
    fftw_free(ss);
}
extern "C"{
#include <malloc.h>
}
#endif
