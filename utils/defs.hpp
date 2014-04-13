#ifndef DEF_H
#define DEF_H
#ifdef CLANG
//this allows Eigen to compile under -Wall
#pragma clang diagnostic ignored "-Wdeprecated-register"
//Not every function uses the full interface
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include <complex>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
//#include "../eigen3/Eigen/Eigen"
#include <memory>
#ifdef ICC
//only worrying about mega-aligned stuff like this for icc, rest is blank for easier compatibility
#define restr restrict
#define ALIGNED(x) __assume_aligned(x, 16)
#define MAKE_ALIGNED __declspec(align(16))
#else
#define restr __restrict__
#define ALIGNED(x) 
#define MAKE_ALIGNED
#endif
//#define restr
//typedef complex<double> comp;
typedef std::complex<double> comp;
const static comp Id = comp(0, 1);
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
class item;
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
void err(std::string message, std::string function, std::string file, _fatal f);
void err(std::string message, std::string function, std::string file, _warning w);
void err(std::string message, std::string function, std::string file, item* p, _fatal f);
void err(std::string message, std::string function, std::string file, item* p, _warning f);
void err(std::string message, std::string function, std::string file, std::shared_ptr<item> p, _fatal f);
void err(std::string message, std::string function, std::string file, std::shared_ptr<item> p, _warning f);


//Alligned malloc
//also ensures return value is valid
void* al_malloc(size_t ss);
void al_free(void* ss);
extern "C"{
#include <malloc.h>
}
#endif
