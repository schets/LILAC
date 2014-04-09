#include "rhs/rhs.h"
#include "rhs_CNLS.h"
#include "comp_funcs.h"
#include <cstring>

comp trap(comp * restr v, size_t s){
    ALIGNED(v);
    comp sum = 0;
#pragma vector aligned
    for(size_t i=1; i < s-1; i++){
        sum += v[i];
    }
    sum += (v[0] + v[s-1])/2.0;
    return sum;
}
double trap(double * restr v, size_t s){
    ALIGNED(v);
    double sum = 0;
#pragma vector aligned
    for(size_t i=1; i < s-1; i++){
        sum += v[i];
    }
    sum += (v[0] + v[s-1])/2.0;
    return sum;
}
/*!
 * Destructor for rhs_CNLS
 * */
rhs_CNLS::~rhs_CNLS(){}

int rhs_CNLS::dxdt(comp* restr x, comp* restr dx, double t){
    uf1= (comp* restr)x;
    uf2=(comp* restr)(x+NUM_TIME_STEPS);
    //take the inverse fourier transform
    ifft(fback, uf1, u1, NUM_TIME_STEPS);
    ifft(fback, uf2, u2, NUM_TIME_STEPS);
    //Inform compiler of alignment, if supported
    ALIGNED(uf1);
    ALIGNED(uf2);
    ALIGNED(u1);
    ALIGNED(u2);
    ALIGNED(sq1);
    ALIGNED(sq2);
    ALIGNED(comp_in_r);
    ALIGNED(comp_in);
    ALIGNED(comp_out);
    ALIGNED(comp_out_r);
    ALIGNED(k);
    ALIGNED(ksq);
#pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        sq1[i] = _sqabs(u1[i]);
        sq2[i] = _sqabs(u2[i]);
        comp_in_r[i] = sq1[i] + sq2[i];
    }
    comp expr1 = Id*(2.0*g0/(1.0+trap(comp_in_r, NUM_TIME_STEPS)*dt/e0));
    //calculate the ffts for the rhs
#pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i] = (sq1[i] + A*sq2[i])*u1[i];
        comp_in[i] = u2[i] * u2[i] * _conj(u1[i]);
    }
    //fourier transform forwards nonlinear equations
    fft(ffor, comp_in, comp_out, NUM_TIME_STEPS);
    fft(ffor, comp_in_r, comp_out_r, NUM_TIME_STEPS);
#pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i] = (((D/2) * ksq[i] + K) * uf1[i] - comp_out_r[i]
                - B*comp_out[i] + expr1*uf1[i]*(1-tau*ksq[i]) - Gamma*uf1[i])/Id;
    }
    //Do the fft work for the other half of the calculations
#pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i] = (A*sq1[i] + sq2[i])*u2[i];
        comp_in[i] = u1[i] * u1[i] * _conj(u2[i]);
    }
    fft(ffor, comp_in, comp_out, NUM_TIME_STEPS);
    fft(ffor, comp_in_r, comp_out_r, NUM_TIME_STEPS);
#pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i+NUM_TIME_STEPS] = (((D/2) * ksq[i] - K) * uf2[i] - comp_out_r[i]
                - B*comp_out[i] + expr1*(uf2[i]-tau*ksq[i]*uf2[i]) - Gamma*uf2[i])/Id;
    }
    return 0;
}

std::vector<std::string> rhs_CNLS::dependencies() const{
    std::string deps[] = {"g0", "e0", "t_int"};
    return appendvec(std::vector<std::string>(deps, deps+3), rhs::dependencies());
}

std::string rhs_CNLS::type() const {
    return "rhs_CNLS";
}
/*!
 * Initializes the rhs_CNLS class  
 */
void rhs_CNLS::postprocess(std::map<std::string, std::shared_ptr<item> >& dat){
    rhs::postprocess(dat);
    NUM_TIME_STEPS = dimension/2;
    if(NUM_TIME_STEPS*2 != dimension){
        err("dimension not even, which is required for rhs_CNLS", 
                "rhs_CNLS::postprocess", "rhs/rhs_CNLS.cpp", FATAL_ERROR);
    }
    dat["t_int"]->retrieve(&LENGTH_T, this);
    if(LENGTH_T <= 0){
        std::string errmess = "t_int is invalid, must be >= 0";
        err(errmess, "rhs_CNLS::postprocess", "rhs/rhs_CNLS.cpp",
                dat["t_int"], FATAL_ERROR);
    }
    dt = LENGTH_T/NUM_TIME_STEPS;
    dat["g0"]->retrieve(&g0, this);
    dat["e0"]->retrieve(&e0, this);
    memp.create(NUM_TIME_STEPS, &u1, &u2, &comp_in, &comp_in_r, &comp_out, &comp_out_r, &sq1, &sq2, &k, &ksq);
    //create k values

    double mulval=(2.0*PI/LENGTH_T)*(NUM_TIME_STEPS/2.0);
    for(int i=0; i<NUM_TIME_STEPS/2; i++){
        k[i] = mulval * (2.0*i/(1.0*NUM_TIME_STEPS));
        ksq[i] = k[i]*k[i];
    }
    for(int i=NUM_TIME_STEPS/2; i<NUM_TIME_STEPS; i++){
        k[i] = mulval * 2.0*(i-(int)NUM_TIME_STEPS)/(NUM_TIME_STEPS*1.0);
        ksq[i] = k[i]*k[i];
    }

}
void rhs_CNLS::parse(std::string inval){
};
