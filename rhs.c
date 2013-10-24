#include "rhs.h"
inline real trap(real * restr v, size_t s){
    real sum = 0;
    for(size_t i=0; i < s-1; i++){
        sum += v[i];
    }
    sum *= 2;
    sum += v[0] + v[s-1];
    return sum;
}
int rhs(double t, double* restr curx, double* restr dx, void* params){
    odeparams* p = params;
    //take the inverse fourier transform
    fftw_execute(p->pb1);
    fftw_execute(p->pb2);
    //do fancy math stuff
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        real cur1 = cabs(p->u1[i]);
        real cur2 = cabs(p->u2[i]);
        p->sq1[i] = cur1*cur1;
        p->sq2[i] = cur2*cur2;
        p->comp_in_r[i] = p->sq1[i] + p->sq2[i];
    }
    real expr1 = I*(2*p->g0/(1+trap(p->comp_in_r, NUM_TIME_STEPS)*p->dt));
    //calculate the ffts for the rhs
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i]=p->sq1[i] + A*p->sq2[i];
        comp_in[i] = p->u2[i] * p->u2[i] + conj(p->u1[i]);
    }
    fftw_execute(c_r);
    fftw_execute(c_p);
    for(size_t i = 0; i < (NUM_TIME_STEPS/2) + 1; i++){
        comp cval = ((D/2) * ksq[i] + K) * p->uf1[i] - comp_r[i]*p->u1; 
    return GSL_SUCCESS;
}
