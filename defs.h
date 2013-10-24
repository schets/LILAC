#ifndef DEF_H
#define DEF_H
#include <math.h>
#include <stdio.h>
#include <complex.h>
#include <fftw3.h>
#include <time.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
//change runtime malloc behavior
//fftw_malloc promises aligned memory for SIMD
//but may be less stable
#define FFTW_MALLOC
#ifdef FFTW_MALLOC
#define ALIGN_BYTES 16
#define malloc fftw_malloc
#define free fftw_free
#else
#include <malloc.h>
#endif
#define restr __restrict__
//#define restr
//This allows easy changing of complex and real types
typedef fftw_complex comp;
typedef double real;
typedef size_t iter;
#define NUM_TIME_STEPS (256) //number of time points
#define LENGTH_T (60) //length of t-domain
//parameters taken from matlab main file
//made as defines to assist with optimization
#define D (-.04)
#define K (.01)
#define A (2/3)
#define tau (.01)
#define Gamma (.01)
#define RTlength (1.5)
#define PI (3.14159)
#define maxTrips 50
#define nl printf("\n")

#endif
