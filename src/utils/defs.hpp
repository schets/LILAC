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
#include <memory>
//some commands to help with effeciency
#ifdef ICC
#define restr restrict
#define ALIGNED(x) __assume_aligned(x, 16)
#define MAKE_ALIGNED __declspec(align(16))
#define PREDICT(x, val) __builtin_expect(x, val)
#define final_def
#endif

#ifdef CLANG
#define final_def final
#define restr __restrict__
#define ALIGNED(x)
#define PREDICT(x, val) x
#define MAKE_ALIGNED alignas(16)
#endif

#ifdef GCC
#define final_def final
#define restr __restrict__
#define ALIGNED(x)
#define PREDICT(x, val) __builtin_expect(x, val)
#define MAKE_ALIGNED
#define alignas(x)
#endif
class item;
typedef std::complex<double> comp;
const static comp Id = comp(0, 1);
//#define NUM_TIME_STEPS (256) //number of time points
//#define LENGTH_T (60) //length of t-domain
//parameters taken from matlab main file
//can move to input parameters as well
//but for now these are defines
constexpr double D =-.4;
constexpr double K =(.1);
constexpr double A =(2.0/3.0);
constexpr double B =(1.0/3.0); //need to find a real value for this
constexpr double tau= (.1);
constexpr double Gamma= (.1);
constexpr double RTlength= (1.5);
constexpr double PI =(3.14159);
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
void trim(std::string& curline, const std::string& val = "\n\r\t ");
void ltoken(std::string& tok, std::string& str, std::string delim=" ");
std::string get_unique_name(std::string base = "");
extern "C"{
#include <malloc.h>
}
#endif
