#include "rhs/rhs.h"
#include "rhs_imp.h"
#include "defs.h"
#include "comp_funcs.h"
#include <cstring>
void printar(comp* u0, comp* u1, int num){
    for(int i = 0; i < num; i++){
        std::cout<<_real(u0[i])<<"+"<< _imag(u0[i])<<"i, "<<_real(u1[i])<<"+"<<_imag(u1[i])<<"i\n";
    }
}
void printar(double* u0, double* u1, int num){
    for(int i = 0; i < num; i++){
        std::cout<<u0[i]<<", "<<u1[i]<<std::endl;
    }
}
void printar(double* u0, int num){
    for(int i = 0; i < num; i++){
        std::cout<<u0[i]<<std::endl;
    }
}
inline void printv(comp c){
    std::cout << _real(c)<<"+"<<_imag(c)<<"i\n";
}
inline void printv(double c){
    std::cout << c << std::endl;
}
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
rhs_CNLS::rhs_CNLS(size_t dimen, double g, double e, double _dt,
        double tlen, size_t n_steps): g0(g), e0(e), dt(_dt),
        LENGTH_T(tlen), NUM_TIME_STEPS(n_steps){
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
        k[i] = 2.0*mulval * i/((float)NUM_TIME_STEPS);
        ksq[i] = k[i]*k[i];
    }
    for(int i=NUM_TIME_STEPS/2; i<NUM_TIME_STEPS; i++){
        k[i] = 2.0*mulval * ((int)i-(int)NUM_TIME_STEPS)/((float)NUM_TIME_STEPS);
        ksq[i] = k[i]*k[i];
    }
}
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


//This is the RHS for the CNLS equations
//I haven't implemented real ffts for the parts to which they apply
//since they have a more complicated output and I just want to have a working
//copy for now
int rhs_CNLS::dxdt(comp* restr x, comp* restr dx, double t){
    uf1=x;
    uf2=x+NUM_TIME_STEPS;
    //take the inverse fourier transform
    ifft(fback, uf1, u1, NUM_TIME_STEPS);
    ifft(fback, uf2, u2, NUM_TIME_STEPS);
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        sq1[i] = _sqabs(u1[i]);
        sq2[i] = _sqabs(u2[i]);
        comp_in_r[i] = sq1[i] + sq2[i];
    }
    comp expr1 = I*(2*g0/(1+trap(comp_in_r, NUM_TIME_STEPS)*dt/e0));
    printv(expr1);
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
        k[i] = 2.0*mulval * i/((float)NUM_TIME_STEPS);
        ksq[i] = k[i]*k[i];
    }
    for(int i=NUM_TIME_STEPS/2; i<NUM_TIME_STEPS; i++){
        k[i] = 2.0*mulval * ((int)i-(int)NUM_TIME_STEPS)/((float)NUM_TIME_STEPS);
        ksq[i] = k[i]*k[i];
    }

}
