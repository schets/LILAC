#include "rhs_SQGLE.h"
#include "utils/inline_trig.h"
#include "utils/comp_funcs.hpp"
#include "controller/controller.h"
#include "types/type_register.hpp"
template class type_register<rhs_SQGLE>;
/*!
 * Destructor for rhs_SQGLE
 * */
rhs_SQGLE::~rhs_SQGLE(){}

inline comp _clog(comp inval){
    return 0.5*log(_sqabs(inval)) + Id*atan2(_imag(inval), _real(inval));
}

//This is still a hacky class, proably won't see much use anywhere
//since it is slow. As I result, clenaign it up isn't a big priority


int rhs_SQGLE::dxdt(ptr_passer x, ptr_passer _dx, double t){
    comp* restr dx = _dx;
    uf1= x;
    //take the inverse fourier transform
    ifft(uf1, u1, NUM_TIME_STEPS);
    //Inform compiler of alignment, if supported
    ALIGNED(uf1);
    ALIGNED(u1);
    ALIGNED(sq1);
    ALIGNED(comp_in);
    ALIGNED(comp_out);
    ALIGNED(k);
    ALIGNED(ksq);

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        sq1[i] = _sqabs(u1[i]);
    }
    comp expr1 = (2.0*g0/(1.0+trap(sq1, NUM_TIME_STEPS)*dt/e0));
    //calculate the ffts, helper arrays for the rhs
    double help_mul=B*sin(2*a1-ap);
    comp ce1 = std::exp(Id*K*(-1.0));
    comp ce2 = std::exp(Id*K);
    ce1=ce1*(cos(2*(a2-a3)-ap)+Id*cos(2*a3-ap));
    ce2=ce2*(sin(2*(a2-a3)-ap)+Id*sin(2*a3-ap));
    float trigvals1[4] __attribute__((aligned(16)));
    float trigvals2[4] __attribute__((aligned(16)));
    v4sf _trigout1 __attribute__((aligned(16)));
    v4sf _trigout2 __attribute__((aligned(16)));
    float* restr trigout1, * restr trigout2;
    //vector inlines for the win

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
        comp_in[i] = ce1*(Id*(double)trigout1[0] - (double)trigout2[0]);
        comp_in[i+1] = ce1*(Id*(double)trigout1[1] - (double)trigout2[1]);
        comp_in[i+2] = ce1*(Id*(double)trigout1[2] - (double)trigout2[2]);
        comp_in[i+3] = ce1*(Id*(double)trigout1[3] - (double)trigout2[3]);

        _trigout1=sin_ps(*(v4sf*)&trigvals1);
        _trigout2=sin_ps(*(v4sf*)&trigvals2);


        comp_in[i] += ce2*(Id*(double)trigout2[0] - (double)trigout1[0]);
        comp_in[i+1] += ce2*(Id*(double)trigout2[1] - (double)trigout1[1]);
        comp_in[i+2] += ce2*(Id*(double)trigout2[2] - (double)trigout1[2]);
        comp_in[i+3] += ce2*(Id*(double)trigout2[3] - (double)trigout1[3]);


        i+=4;
    }


    //can just reuse trigout here
    for(size_t i = 0; i < NUM_TIME_STEPS;){
        trigvals1[0] = _sqabs(comp_in[i]);
        trigvals1[1] = _sqabs(comp_in[i+1]);
        trigvals1[2] = _sqabs(comp_in[i+2]);
        trigvals1[3] = _sqabs(comp_in[i+3]);
        _trigout1 = log_ps(*(v4sf*)&trigvals1);
        comp_in[i] = 0.5*trigout1[0] + atan2(_imag(comp_in[i]), _real(comp_in_r[i]))*Id + log(0.5);
        comp_in[i+1] = 0.5*trigout1[1] + atan2(_imag(comp_in[i+1]), _real(comp_in[i+1]))*Id + log(0.5);
        comp_in[i+2] = 0.5*trigout1[2] + atan2(_imag(comp_in[i+2]), _real(comp_in[i+2]))*Id + log(0.5);
        comp_in[i+3] = 0.5*trigout1[3] + atan2(_imag(comp_in[i+3]), _real(comp_in[i+3]))*Id + log(0.5);
        i+= 4;
    }

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        //fft helper
        comp_in[i] = u1[i]*Id*(expr1 + comp_in[i] + Id*sq1[i]-Gamma);
    }
    //fourier transform forwards nonlinear equations
    fft(comp_in, comp_out, NUM_TIME_STEPS);

    for(size_t i = 0; i < NUM_TIME_STEPS; i++){
        dx[i] = -1.0*Id*((D/2 - Id * expr1*tau)*ksq[i]*uf1[i] + comp_out[i]);
    }
    return 0;
}

std::vector<std::string> rhs_SQGLE::dependencies() const{
    std::string deps[] = {"g0", "e0", "t_int", "controller"};
    return make_append(deps, rhs::dependencies());
}

std::string rhs_SQGLE::type() const {
    return "SQGLE";
}
/*!
 * Initializes the rhs_SQGLE class  
 */
void rhs_SQGLE::postprocess(input& dat){
    rhs::postprocess(dat);
    NUM_TIME_STEPS = dimension;
    retrieve(LENGTH_T, dat["t_int"], this);
    if(LENGTH_T <= 0){
        std::string errmess = "t_int is invalid, must be >= 0";
        err(errmess, "rhs_SQGLE::postprocess", "rhs/rhs_SQGLE.cpp",
                dat["t_int"], FATAL_ERROR);
    }
    dt = LENGTH_T/NUM_TIME_STEPS;
    retrieve(g0, dat["g0"], this);
    retrieve(e0, dat["e0"], this);
    memp.create(dimension, &u1, &u2, &comp_in, &comp_out, &sq1, &ksq, &k);
    //create k values

    double mulval=(2.0*PI/LENGTH_T)*(NUM_TIME_STEPS/2.0);
    for(size_t i=0; i<NUM_TIME_STEPS/2; i++){
        k[i] = mulval * (2.0*i/(1.0*NUM_TIME_STEPS));
        ksq[i] = k[i]*k[i];
    }
    for(size_t i=NUM_TIME_STEPS/2; i<NUM_TIME_STEPS; i++){
        k[i] = mulval * 2.0*(i-(int)NUM_TIME_STEPS)/(NUM_TIME_STEPS*1.0);
        ksq[i] = k[i]*k[i];
    }
    controller* cont;
    retrieve(cont, dat["controller"], this);
    a1=a2=a3=ap=0;
    std::shared_ptr<variable> vv = std::make_shared<variable>();
    vv->holder=holder;
    vv->setname("a1");
    vv->set(a1);
    vv->parse("0.1");
    dat[vv->name()]=vv;
    retrieve(a1, vv, this);
    cont->addvar(vv);

    //a2
    vv = std::make_shared<variable>();
    vv->holder=holder;
    vv->setname("a2");
    vv->set(a2);
    vv->parse("0.1");
    dat[vv->name()]=vv;
    cont->addvar(vv);
    retrieve(a2, vv, this);
    //a3
    vv = std::make_shared<variable>();
    vv->holder=holder;
    vv->setname("a3");
    vv->set(a3);
    vv->parse("0.1");
    dat[vv->name()]=vv;
    cont->addvar(vv);
    retrieve(a3, vv, this);
    //ap
    vv = std::make_shared<variable>();
    vv->holder=holder;
    vv->setname("ap");
    vv->set(ap);
    vv->parse("0.1");
    dat[vv->name()]=vv;
    cont->addvar(vv);
    retrieve(ap, vv, this); 
    //temporary solution, add variables directly to controller
}
void rhs_SQGLE::update(){}
