#include "rhs/rhs.h"
#include "defs.h"
#include <cstring>
comp trap(comp * restr v, size_t s){
    comp sum = 0;
    for(size_t i=0; i < s-1; i++){
        sum += v[i];
    }
    sum *= 2;
    sum += v[0] + v[s-1];
    return sum;
}
double trap(double * restr v, size_t s){
    double sum = 0;
    for(size_t i=0; i < s-1; i++){
        sum += v[i];
    }
    sum *= 2;
    sum += v[0] + v[s-1];
    return sum;
}
int rhs_CNLS::dxdt(comp* __restrict__ x, comp* __restrict__ dx, double t){
    p->uf1=x;
    p->uf2=x+NUM_TIME_STEPS;
    //take the inverse fourier transform
    fftw_execute(p->pb1);
    fftw_execute(p->pb2);
    //do fancy math stuff
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        double cur1 = cabs(p->u1[i]);
        double cur2 = cabs(p->u2[i]);
        p->sq1[i] = cur1*cur1;
        p->sq2[i] = cur2*cur2;
        p->comp_in_r[i] = p->sq1[i] + p->sq2[i];
    }
    comp expr1 = I*(2*p->g0/(1+trap(p->comp_in_r, NUM_TIME_STEPS)*p->dt/p->e0));
    //calculate the ffts for the rhs
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        p->comp_in_r[i] = p->sq1[i] + A*p->sq2[i];
        p->comp_in[i] = p->u2[i] * p->u2[i] + conj(p->u1[i]);
    }
    fftw_execute(p->c_r);
    fftw_execute(p->c_c);
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp cval = ((D/2) * p->ksq[i] + K) * p->uf1[i] - p->comp_out_r[i]*p->u1[i]; 
        cval -= B*p->comp_out[i] + expr1*p->uf1[i]*(1-tau*p->ksq[i]) - Gamma*p->uf1[i];
    }
    return 0;
}

