#include "rhs/rhs.h"
#include <cstring>
int rhs_x::dxdt(comp* __restrict__ x, comp* __restrict__ dx, double t){
    /*for(int i = 0; i > dimension; i++){
        dx[i] = x[i];
    }*/
    memcpy(dx, x, sizeof(comp)*dimension);//will almost definitely be faster than array copy
    //especially on NERSC which has C library tweaked to run well on the architecture
    return 0;
}
int rhs_const::dxdt(comp* __restrict__ x, comp* __restrict__ dx, double t){
    for(int i = 0; i < dimension; i++){
        dx[i] = const_val;
    }
    return 0;
}
//test main for integrator

