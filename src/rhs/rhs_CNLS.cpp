#include "rhs_CNLS.h"
#include "comp_funcs.hpp"
#include "utils/ptr_passer.hpp"
#include "types/type_register.hpp"

template class type_register<rhs_CNLS>;
/*!
 * Destructor for rhs_CNLS
 * */
rhs_CNLS::~rhs_CNLS(){}
    
int rhs_CNLS::dxdt(ptr_passer x, ptr_passer _dx, double t){
    comp* restr dx = _dx.get<comp>();
    uf1= x.get<comp>();
    uf2=uf1+NUM_TIME_STEPS;
    //take the inverse fourier transform
    ifft(uf1, u1, NUM_TIME_STEPS);
    ifft(uf2, u2, NUM_TIME_STEPS);
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

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        sq1[i] = _sqabs(u1[i]);
        sq2[i] = _sqabs(u2[i]);
        comp_in_r[i] = sq1[i] + sq2[i];
    }
    comp expr1 = Id*(2.0*g0/(1.0+trap(comp_in_r, NUM_TIME_STEPS)*dt/e0));
    //calculate the ffts for the rhs

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i] = (sq1[i] + A*sq2[i])*u1[i];
        comp_in[i] = u2[i] * u2[i] * _conj(u1[i]);
    }
    //fourier transform forwards nonlinear equations
    fft(comp_in, comp_out, NUM_TIME_STEPS);
    fft(comp_in_r, comp_out_r, NUM_TIME_STEPS);

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i] = (((D/2) * ksq[i] + K) * uf1[i] - comp_out_r[i]
                - B*comp_out[i] + expr1*uf1[i]*(1-tau*ksq[i]) - Gamma*uf1[i])/Id;
    }
    //Do the fft work for the other half of the calculations

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i] = (A*sq1[i] + sq2[i])*u2[i];
        comp_in[i] = u1[i] * u1[i] * _conj(u2[i]);
    }
    fft(comp_in, comp_out, NUM_TIME_STEPS);
    fft(comp_in_r, comp_out_r, NUM_TIME_STEPS);

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i+NUM_TIME_STEPS] = (((D/2) * ksq[i] - K) * uf2[i] - comp_out_r[i]
                - B*comp_out[i] + expr1*(uf2[i]-tau*ksq[i]*uf2[i]) - Gamma*uf2[i])/Id;
    }
    return 0;
}

std::vector<std::string> rhs_CNLS::dependencies() const{
    std::string deps[] = {"g0", "e0", "t_int"};
    return make_append(deps, rhs::dependencies());
}

std::string rhs_CNLS::type() const {
    return "CNLS";
}
/*!
 * Initializes the rhs_CNLS class  
 */
void rhs_CNLS::postprocess(input& dat){
    rhs::postprocess(dat);
    NUM_TIME_STEPS = dimension/2;
    if(NUM_TIME_STEPS*2 != dimension){
        err("dimension not even, which is required for rhs_CNLS", 
                "rhs_CNLS::postprocess", "rhs/rhs_CNLS.cpp", FATAL_ERROR);
    }
    retrieve(LENGTH_T, dat["t_int"], this);
    if(LENGTH_T <= 0){
        std::string errmess = "t_int is invalid, must be >= 0";
        err(errmess, "rhs_CNLS::postprocess", "rhs/rhs_CNLS.cpp",
                dat["t_int"], FATAL_ERROR);
    }
    dt = LENGTH_T/NUM_TIME_STEPS;
    retrieve(g0, dat["g0"], this);
    retrieve(e0, dat["e0"], this);
    memp.create(NUM_TIME_STEPS, &u1, &u2, &comp_in, &comp_in_r, &comp_out, &comp_out_r, &sq1, &sq2, &k, &ksq);
    //create k values
    double mulval=(2.0*PI/LENGTH_T)*(NUM_TIME_STEPS/2.0);
    for(size_t i=0; i<NUM_TIME_STEPS/2; i++){
        k[i] = mulval * (2.0*i/(1.0*NUM_TIME_STEPS));
        ksq[i] = k[i]*k[i];
    }
    for(size_t i=NUM_TIME_STEPS/2; i<NUM_TIME_STEPS; i++){
        k[i] = mulval * 2.0*((int)i-(int)NUM_TIME_STEPS)/(NUM_TIME_STEPS*1.0);
        ksq[i] = k[i]*k[i];
    }

}
