#ifndef DEF_H
#define DEF_H
#include <iostream>
extern "C" {
#include <complex.h>
#include <fftw3.h>
#include <math.h>
#include <time.h>
}
//change runtime malloc behavior
//fftw_malloc promises aligned memory for SIMD
//and is therefore used intead of new/malloc
#define FFTW_MALLOC
#ifdef FFTW_MALLOC
#define malloc fftw_malloc
#define free fftw_free
#else
extern "C"{
#include <malloc.h>
}
#endif
#define restr __restrict__
//#define restr
using namespace std;
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
#define tau (.01)
#define Gamma (.1)
#define RTlength (1.5)
#define PI (3.14159)
#define maxTrips 50
#endif
