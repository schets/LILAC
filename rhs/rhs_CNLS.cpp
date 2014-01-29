#include "rhs/rhs.h"
#include "rhs_imp.h"
#include "defs.h"
#include "comp_funcs.h"
#include <cstring>

comp trap(comp * restr v, size_t s){
    comp sum = 0;
    for(size_t i=1; i < s-1; i++){
        sum += v[i];
    }
    sum += (v[0] + v[s-1])/2.0;
    return sum;
}
double trap(double * restr v, size_t s){
    double sum = 0;
    for(size_t i=1; i < s-1; i++){
        sum += v[i];
    }
    sum += (v[0] + v[s-1])/2.0;
    return sum;
}
/*!
 * Destructor for rhs_CNLS
 * */
rhs_CNLS::~rhs_CNLS(){
    free(u1);
    free(u2);
    free(comp_in);
    free(comp_out);
    free(comp_in_r);
    free(comp_out_r);
    free(sq1);
    free(sq2);
    free(k);
    free(ksq);
    fftw_destroy_plan(ffor);
    fftw_destroy_plan(fback);
}

int rhs_CNLS::dxdt(comp* restr x, comp* restr dx, double t){

    uf1= (comp* restr)x;
    uf2=(comp* restr)(x+NUM_TIME_STEPS);
    u1 = (comp* restr)u1;
    u2 = (comp* restr)u2;
    //take the inverse fourier transform
    ifft(fback, uf1, u1, NUM_TIME_STEPS);
    ifft(fback, uf2, u2, NUM_TIME_STEPS);
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        sq1[i] = _sqabs(u1[i]);
        sq2[i] = _sqabs(u2[i]);
        comp_in_r[i] = sq1[i] + sq2[i];
    }
    comp expr1 = I*(2*g0/(1+trap(comp_in_r, NUM_TIME_STEPS)*dt/e0));
    //calculate the ffts for the rhs
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i] = (sq1[i] + A*sq2[i])*u1[i];
        comp_in[i] = u2[i] * u2[i] * _conj(u1[i]);
    }
    //fourier transform forwards nonlinear equations
    fft(ffor, comp_in, comp_out, NUM_TIME_STEPS);
    fft(ffor, comp_in_r, comp_out_r, NUM_TIME_STEPS);
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i] = (((D/2) * ksq[i] + K) * uf1[i] - comp_out_r[i]
                - B*comp_out[i] + expr1*uf1[i]*(1-tau*ksq[i]) - Gamma*uf1[i])/I;
    }
    //Do the fft work for the other half of the calculations

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        comp_in_r[i] = (A*sq1[i] + sq2[i])*u2[i];
        comp_in[i] = u1[i] * u1[i] * _conj(u2[i]);
    }
    fft(ffor, comp_in, comp_out, NUM_TIME_STEPS);
    fft(ffor, comp_in_r, comp_out_r, NUM_TIME_STEPS);
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i+NUM_TIME_STEPS] = (((D/2) * ksq[i] - K) * uf2[i] - comp_out_r[i]
                - B*comp_out[i] + expr1*(uf2[i]-tau*ksq[i]*uf2[i]) - Gamma*uf2[i])/I;
    }
    //printar(dx, NUM_TIME_STEPS*2);
    //
    //all values have been set
    //return success code
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
void rhs_CNLS::postprocess(std::map<std::string, item*>& dat){
    rhs::postprocess(dat);
    NUM_TIME_STEPS = dimension/2;
    if(NUM_TIME_STEPS*2 != dimension){
        err("dimension not even, which is required for rhs_CNLS", 
                "rhs_CNLS::postprocess", "rhs/rhs_CNLS.cpp", FATAL_ERROR);
    }
    dat["t_int"]->retrieve(&LENGTH_T);
    if(LENGTH_T <= 0){
        std::string errmess = "t_int is invalid, must be >= 0";
        err(errmess, "rhs_CNLS::postprocess", "rhs/rhs_CNLS.cpp",
                dat["t_int"], FATAL_ERROR);
    }
    dt = LENGTH_T/NUM_TIME_STEPS;
    dat["g0"]->retrieve(&g0);
    dat["e0"]->retrieve(&e0);
    u1 = (comp*)malloc(NUM_TIME_STEPS*sizeof(comp));
    u2 = (comp*)malloc(NUM_TIME_STEPS*sizeof(comp));
    comp_in = (comp*)malloc(NUM_TIME_STEPS*sizeof(comp));
    comp_out = (comp*)malloc(NUM_TIME_STEPS*sizeof(comp));
    comp_out_r = (comp*)malloc(NUM_TIME_STEPS*sizeof(comp));
    comp_in_r = (comp*)malloc(NUM_TIME_STEPS*sizeof(comp));
    sq1 = (double*)malloc(NUM_TIME_STEPS*sizeof(double));
    sq2 = (double*)malloc(NUM_TIME_STEPS*sizeof(double));
    k = (double*)malloc(NUM_TIME_STEPS*sizeof(double));
    ksq = (double*)malloc(NUM_TIME_STEPS*sizeof(double));
    //input arrays don't really matter here because the plan
    //is executed against specific input arrays at runtime
    //and not with the initialization arrays
    ffor=fftw_plan_dft_1d(NUM_TIME_STEPS, u1, u2,FFTW_FORWARD, FFTW_ESTIMATE); 
    fback=fftw_plan_dft_1d(NUM_TIME_STEPS, comp_in, comp_out, FFTW_BACKWARD, FFTW_ESTIMATE); 
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
