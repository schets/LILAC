#ifndef COMP_FUNCS_H
#define COMP_FUNCS_H
#include <math.h>
//THis file contains inlined complex functions
//The C and C++ standard treat complex as an array of two doubles
//but the C standard doesn't always have inlined complex functions
//and the C++ standard uses templates and wreaks havoc with
//the compiler
//So these operate directly on the double values in the array
//and provide nice inlined functions that should also
//work nicely with SIMD instructions
inline comp _conj(comp inval){
    ((double*)&inval)[1] *= -1;
    return inval;
}
inline double _real(comp inval){
    return ((double*)&inval)[0];
}
inline double _imag(comp inval){
    return ((double*)&inval)[1];
}
inline double _abs(comp inval){
    double r, i;
    r = ((double*)&inval)[0];
    i = ((double*)&inval)[1];
    return sqrt(r*r + i*i);
}
inline double _sqabs(comp inval){
    double r, i;
    r = ((double*)&inval)[0];
    i = ((double*)&inval)[1];
    return r*r + i*i;
}
#endif
