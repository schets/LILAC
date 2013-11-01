#ifndef DEF_H
#define DEF_H
#include <cmath>
#include <iostream>
#include <complex.h>
extern "C" {
#include <fftw3.h>
}
#include <ctime>
//change runtime malloc behavior
//fftw_malloc promises aligned memory for SIMD
//and is therefore used intead of new/malloc
#define FFTW_MALLOC
#ifdef FFTW_MALLOC
#define malloc fftw_malloc
#define free fftw_free
#else
extern "C"{
#include <comnplex.h>
#include <malloc.h>
}
#endif
#define restr __restrict__
//#define restr
using namespace std;
//typedef complex<double> comp;
typedef fftw_complex comp;
#define NUM_TIME_STEPS (256) //number of time points
#define LENGTH_T (60) //length of t-domain
//parameters taken from matlab main file
//made as defines to assist with optimization
#define D (-.04)
#define K (.01)
#define A (2/3)
#define B (1) //need to find a real value for this
#define tau (.01)
#define Gamma (.01)
#define RTlength (1.5)
#define PI (3.14159)
#define maxTrips 50
typedef struct odeparams{
    comp * restr u1, * restr u2, * restr uf1, *restr uf2, * restr comp_in,
         * restr comp_out, * restr comp_out_r, * restr comp_in_r;
    double * restr sq1, * restr sq2, * restr k, * restr ksq;
    fftw_plan p1, p2, pb1, pb2, c_r, c_c;
    double alpha1, alpha2, alpha3, alpha4;
    double g0, e0, dt;
} odeparams;
#endif
