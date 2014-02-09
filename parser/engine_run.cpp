#include "engineimp.h"
#include "engine.h"
#include <iostream>
#include "utils/comp_funcs.h"
#include "rhs/rhs.h"
#include "rhs/rhs_imp.h"
#include "integrator/integrator.h"
#include "controller/controller.h"
#include "simulation/simulation.h"
#include <time.h>
#include <stdio.h>
#include <cmath>
double mom4(comp invals[], double* kvals, int len){
    double mom = 0;
    double mval = 0;
    //if kvals are not passed, just use integer frequencies
    //since it is only a multiplying factor
    if(kvals){
        for(int i = 0; i < len; i++){
            mval += _abs(invals[i]);
        }
        mval /= len;
        for(int i = 0; i < len; i++){
            mom += pow(_sqabs(invals[i]), 2)*kvals[i];
        }
    }
    else{

        for(int i = 0; i < len/2; i++){
            mom += pow(_sqabs(invals[i]), 2)* i;//*kvals[i];
        }
        for(int i = len/2; i < len; i++){
            mom += pow(_sqabs(invals[i]), 2)* (i - len);//*kvals[i];
        }

    }
    return mom;
}

//This calculates the moments of a distribution
void make_torxyz(double c1, double c2, double rb, double rs, double* out){
    out[0]=cos(c1)*(cos(c2)+rb);
    out[1]=sin(c1)*(cos(c2)+rb);
    out[2]=sin(c2);
}

inline Eigen::Matrix<double, 2, 2> rmat(double alpha){
    Eigen::Matrix<double, 2, 2> in;
    in(0, 0) = cos(alpha);
    in(0, 1) = -1*sin(alpha);
    in(1, 0)=sin(alpha);
    in(1, 1)=cos(alpha);
    return in;
}
void jac(comp* jm, comp* x, comp* store, comp* store2, rhs* rh){
    for(size_t i = 0; i < rh->dimension; i++)
    {
        const double diff = .0001;
        x[i] += diff;
        rh->dxdt(x, store, 0);
        x[i] -= diff*2;
        rh->dxdt(x, store, 0);
        for(size_t j = 0; j < rh->dimension; j++){
            jm[j+rh->dimension*i]=(store[j]-store2[j])/(2*diff);
        }
    };
}
void print2c(Eigen::Matrix<comp, 2, 2>& in){
    printf("%lf+%lfi, %lf+%lfi\n", _real(in(0, 0)), _imag(in(0, 0)), _real(in(0, 1)), _imag(in(0, 1)));
    printf("%lf+%lfi, %lf+%lfi\n\n", _real(in(1, 0)), _imag(in(1, 0)), _real(in(1, 1)), _imag(in(1, 1)));
}
void engineimp::run(){
    int ints;
    values["dimension"]->retrieve(&ints, 0);
    size_t nts=(size_t)ints;
    nts/= 2;
    double t_int;
    values["t_int"]->retrieve(&t_int, 0);
    if(!values.count("integrator")){
        err("integrator variable is not defined", "engineimp::run()",
                "parser/engine_run.cpp", FATAL_ERROR);
    }
    if(!values.count("rhs")){
        err("rhs variable is not defined", "engineimp::run()",
                "parser/engine_run.cpp", FATAL_ERROR);
    }
    Eigen::Matrix<comp, 2, 2> wq, wh, wp, j1, j2, j3, jp;
    Eigen::Matrix<double, 2, 2> jcalc;
    integrator* inter;
    values["integrator"]->retrieve(&inter, 0);
    int num_steps;
    values["tor_steps"]->retrieve(&num_steps, 0);
    rhs* rh;
    values["rhs"]->retrieve(&rh, 0);
    simulation* sys;
    values["simulation"]->retrieve(&sys, 0);
    comp* restr u0 = (comp*)al_malloc(2*nts*sizeof(comp));
    comp* restr u1 = (comp*)al_malloc(2*nts*sizeof(comp));
    comp* kurtosis_help = (comp*)al_malloc(nts*sizeof(comp));
    double* help = (double*)al_malloc(nts*sizeof(double));
    double* mhold = (double*)al_malloc(nts*sizeof(double));
    double* phold = (double*)al_malloc(nts*sizeof(double));
    double* t = (double*)al_malloc((nts)*sizeof(double)); 
    double dt =t_int*1.0/nts;
    comp m1[4], m2[4], m3[4], m4[4];
    for(int i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }

    for(int i = 0; i < nts; i++){
        u0[i] = u0[i+nts] = 1.00/cosh(t[i]/2.0);
        help[i] = _real(u0[i]);
    }
    FILE* f = fopen("iso_out.3D", "w");
    fprintf(f, "X Y Z val\n");
    fftw_plan t1 = fftw_plan_dft_1d(nts, u0, u0, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_plan t2 = fftw_plan_dft_1d(nts, u0+nts, u0+nts, FFTW_FORWARD, FFTW_ESTIMATE);
    clock_t tval =clock();
    Eigen::Map<Eigen::Matrix<comp, 2, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Aligned> dmap(u0, 2, nts);
    double a1, a2, a3, ap;
    wq(1, 0) = wq (0, 1) = 0;
    wq(0, 0)=(comp)cexp(-1*I*3.14159*0.25);
    wq(1, 1)=(comp)cexp(1.0*I*3.14159*0.25);
    wh(1, 0)=wh(0, 1)=0;
    wh(0, 0)=-1.0*I;
    wh(1, 1)=1.0*I;
    wp(0, 1)=wp(1, 0)=0;
    wp(0, 0)=1;
    wp(1, 1)=0;
    double mscore = 0;
    double mpulse = 0;
    double mener = 0;
    FILE* fmax = fopen("max.out", "w");
    FILE* fpulse = fopen("pmax.out", "w");
    a3=.4860;
    controller* cont;
    values["controller"]->retrieve(&cont, 0);
    double xyz[3];
    int count=0;
    double ener;

    double norm, change_norm;
    norm = 0;
    for(int qq = 0; qq < num_steps; qq++){
       double score = sys->simulate();
       cont->control(0, 0);
       update();
    }
    for(int j = 0; j < nts; j++){
//        fprintf(fmax, "%d, %lf\n", j, mhold[j]);
//        fprintf(fpulse, "%d, %lf\n", j, phold[j]);
    }
    fclose(f);
    fclose(fmax);
    fclose(fpulse);
    fftw_destroy_plan(t2);
    fftw_destroy_plan(t1);
    al_free(u0);
    al_free(u1);
    al_free(t);
    al_free(help);
    al_free(mhold);
    al_free(phold);
    al_free(kurtosis_help);
    fftw_cleanup();
}
