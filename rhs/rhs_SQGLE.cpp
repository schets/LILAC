#include "rhs/rhs.h"
#include "rhs_SQGLE.h"
#include "defs.h"
#include "inline_trig.h"
#include "comp_funcs.h"
#include "controller/controller.h"
#include <cstring>

comp trap(comp * restr v, size_t s);
double trap(double * restr v, size_t s);
/*!
 * Destructor for rhs_SQGLE
 * */
rhs_SQGLE::~rhs_SQGLE(){
    al_free(u1);
    al_free(sq1);
    al_free(sq2);
    al_free(k);
    al_free(ksq);
    fftw_destroy_plan(ffor);
    fftw_destroy_plan(fback);
}

inline comp _clog(comp inval){
    return 0.5*log(_sqabs(inval)) + I*atan2(_imag(inval), _real(inval));
}
int rhs_SQGLE::dxdt(comp* restr x, comp* restr dx, double t){

    uf1= (comp* restr)x;
    //take the inverse fourier transform
    ifft(fback, uf1, u1, NUM_TIME_STEPS);
    //Inform compiler of alignment, if supported
    ALIGNED(uf1);
    ALIGNED(u1);
    ALIGNED(sq1);
    ALIGNED(comp_in_r);
    ALIGNED(comp_in);
    ALIGNED(comp_out);
    ALIGNED(comp_out_r);
    ALIGNED(k);
    ALIGNED(ksq);
    #pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        sq1[i] = _sqabs(u1[i]);
    }
    double In=trap(sq1, NUM_TIME_STEPS);
    comp expr1 = (2.0*g0/(1.0+trap(sq1, NUM_TIME_STEPS)*dt/e0));
    //calculate the ffts, helper arrays for the rhs
    double help_mul=B*sin(2*a1-ap);
    comp ce1 = cexp(-1*I*K);
    comp ce2 = cexp(I*K);
    ce1=ce1*(cos(2*(a2-a3)-ap)+I*cos(2*a3-ap));
    ce2=ce2*(sin(2*(a2-a3)-ap)+I*sin(2*a3-ap));
    float avals[4] __attribute__((aligned(16))) = {a1, a2, a3, ap};
    float trigvals1[4] __attribute__((aligned(16)));
    float trigvals2[4] __attribute__((aligned(16)));
    v4sf _trigout1 __attribute__((aligned(16)));
    v4sf _trigout2 __attribute__((aligned(16)));
    float* restr trigout1, * restr trigout2;
        //vector inlines for the win
#pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS;) {
        //try vector inlines on the first one

        trigvals1[0] = 2*a1-ap-sq1[i]*help_mul;
        trigvals1[1] = 2*a1-ap-sq1[i+1]*help_mul;
        trigvals1[2] = 2*a1-ap-sq1[i+2]*help_mul;
        trigvals1[3] = 2*a1-ap-sq1[i+3]*help_mul;
        trigvals2[0] = ap-help_mul*sq1[i];
        trigvals2[1] = ap-help_mul*sq1[i+1];
        trigvals2[2] = ap-help_mul*sq1[i+2];
        trigvals2[3] = ap-help_mul*sq1[i+3];

        _trigout1=cos_ps(*(v4sf*)&trigvals1);
        _trigout2=cos_ps(*(v4sf*)&trigvals2);
        trigout1 = (float*)&_trigout1;
        trigout2 = (float*)&_trigout2;
        comp_in_r[i] = ce1*(I*trigout1[0] - trigout2[0]);
        comp_in_r[i+1] = ce1*(I*trigout1[1] - trigout2[1]);
        comp_in_r[i+2] = ce1*(I*trigout1[2] - trigout2[2]);
        comp_in_r[i+3] = ce1*(I*trigout1[3] - trigout2[3]);

        _trigout1=sin_ps(*(v4sf*)&trigvals1);
        _trigout2=sin_ps(*(v4sf*)&trigvals2);


        comp_in_r[i] += ce2*(I*trigout2[0] - trigout1[0]);
        comp_in_r[i+1] += ce2*(I*trigout2[1] - trigout1[1]);
        comp_in_r[i+2] += ce2*(I*trigout2[2] - trigout1[2]);
        comp_in_r[i+3] += ce2*(I*trigout2[3] - trigout1[3]);


        i+=4;
    }

#pragma vector aligned
    //can just reuse trigout here
    for(size_t i = 0; i < NUM_TIME_STEPS;){
        trigvals1[0] = _sqabs(comp_in_r[i]);
        trigvals1[1] = _sqabs(comp_in_r[i+1]);
        trigvals1[2] = _sqabs(comp_in_r[i+2]);
        trigvals1[3] = _sqabs(comp_in_r[i+3]);
        _trigout1 = log_ps(*(v4sf*)&trigvals1);
        comp_in_r[i] = 0.5*trigout1[0] + atan2(_imag(comp_in_r[i]), _real(comp_in_r[i]))*I + log(0.5);
        comp_in_r[i+1] = 0.5*trigout1[1] + atan2(_imag(comp_in_r[i+1]), _real(comp_in_r[i+1]))*I + log(0.5);
        comp_in_r[i+2] = 0.5*trigout1[2] + atan2(_imag(comp_in_r[i+2]), _real(comp_in_r[i+2]))*I + log(0.5);
        comp_in_r[i+3] = 0.5*trigout1[3] + atan2(_imag(comp_in_r[i+3]), _real(comp_in_r[i+3]))*I + log(0.5);
        i+= 4;
    }
#pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        //fft helper
        comp_in[i] = u1[i]*I*(expr1 + comp_in_r[i] + I*sq1[i]-Gamma);
    }
    //fourier transform forwards nonlinear equations
    fft(ffor, comp_in, comp_out, NUM_TIME_STEPS);
#pragma vector aligned
    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i] = -1*I*((D/2 - I * expr1*tau)*ksq[i]*uf1[i] + comp_out[i]);
    }
    return 0;
}

std::vector<std::string> rhs_SQGLE::dependencies() const{
    std::string deps[] = {"g0", "e0", "t_int", "controller"};
    return appendvec(std::vector<std::string>(deps, deps+4), rhs::dependencies());
}

std::string rhs_SQGLE::type() const {
    return "rhs_SQGLE";
}
/*!
 * Initializes the rhs_SQGLE class  
 */
void rhs_SQGLE::postprocess(std::map<std::string, item*>& dat){
    rhs::postprocess(dat);
    NUM_TIME_STEPS = dimension;
    dat["t_int"]->retrieve(&LENGTH_T, this);
    if(LENGTH_T <= 0){
        std::string errmess = "t_int is invalid, must be >= 0";
        err(errmess, "rhs_SQGLE::postprocess", "rhs/rhs_SQGLE.cpp",
                dat["t_int"], FATAL_ERROR);
    }
    dt = LENGTH_T/NUM_TIME_STEPS;
    dat["g0"]->retrieve(&g0, this);
    dat["e0"]->retrieve(&e0, this);
    u1 = (comp*)al_malloc(NUM_TIME_STEPS*(6*sizeof(comp)));
    u2 = u1+NUM_TIME_STEPS;//(comp*)al_malloc(NUM_TIME_STEPS*sizeof(comp));
    comp_in = u2+NUM_TIME_STEPS;//(comp*)al_malloc(NUM_TIME_STEPS*sizeof(comp));
    comp_out = comp_in+NUM_TIME_STEPS;//(comp*)al_malloc(NUM_TIME_STEPS*sizeof(comp));
    comp_out_r = comp_out+NUM_TIME_STEPS;//(comp*)al_malloc(NUM_TIME_STEPS*sizeof(comp));
    comp_in_r = comp_out_r+NUM_TIME_STEPS;//(comp*)al_malloc(NUM_TIME_STEPS*sizeof(comp));
    sq1 = (double*)al_malloc(NUM_TIME_STEPS*sizeof(double));
    sq2 = (double*)al_malloc(NUM_TIME_STEPS*sizeof(double));
    k = (double*)al_malloc(NUM_TIME_STEPS*sizeof(double));
    ksq = (double*)al_malloc(NUM_TIME_STEPS*sizeof(double));
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
    controller* cont;
    dat["controller"]->retrieve(&cont, this);
    a1=a2=a3=ap=0;
    variable* vv = new variable();
    vv->holder=holder;
    vv->setname("a1");
    vv->set(a1);
    vv->parse("0.1");
    dat[vv->name()]=vv;
    vv->retrieve(&a1, this);
    cont->addvar(vv);
    //a2
    vv = new variable();
    vv->holder=holder;
    vv->setname("a2");
    vv->set(a2);
    vv->parse("0.1");
    dat[vv->name()]=vv;
    cont->addvar(vv);
    vv->retrieve(&a2, this);
    //a3
    vv = new variable();
    vv->holder=holder;
    vv->setname("a3");
    vv->set(a3);
    vv->parse("0.1");
    dat[vv->name()]=vv;
    cont->addvar(vv);
    vv->retrieve(&a3, this);
    //ap
    vv = new variable();
    vv->holder=holder;
    vv->setname("a4");
    vv->set(ap);
    vv->parse("0.1");
    dat[vv->name()]=vv;
    cont->addvar(vv);
    vv->retrieve(&ap, this); 
    //temporary solution, add variables directly to controller
}
void rhs_SQGLE::parse(std::string inval){
}
void rhs_SQGLE::update(){}
