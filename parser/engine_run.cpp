#include "engineimp.h"
#include "engine.h"
#include <iostream>
#include "utils/comp_funcs.h"
#include "rhs/rhs.h"
#include "rhs/rhs_imp.h"
#include "integrator/integrator.h"
#include "controller/controller.h"
#include <time.h>
#include <stdio.h>
extern "C"{
#include <gsl/gsl_statistics_double.h>
#include "pulse_check.h"
}

double mom4(double invals[], size_t len){
    double mean = gsl_stats_mean(invals, 1, len);
    double sd = gsl_stats_sd(invals, 1, len);
    double mom = 0;
    mean = 0;
    for(size_t i = 0; i < len; i++){
        mom += pow((invals[i] - mean)/sd, 4);
    }
    mom /= len;
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
    comp* restr u0 = (comp*)al_malloc(2*nts*sizeof(comp));
    comp* restr u1 = (comp*)al_malloc(2*nts*sizeof(comp));
    double* help = (double*)al_malloc(nts*sizeof(double));
    double* mhold = (double*)al_malloc(nts*sizeof(double));
    double* phold = (double*)al_malloc(nts*sizeof(double));
    double* t = (double*)al_malloc((nts)*sizeof(double)); 
    double dt =t_int*1.0/nts;
    comp m1[4], m2[4], m3[4], m4[4];
    Eigen::Matrix<double, 2, 2> val = rmat(.99);
    Eigen::Map<Eigen::Matrix<comp, 2, 2, Eigen::RowMajor> > em1(m1);
    Eigen::Map<Eigen::Matrix<comp, 2, 2, Eigen::RowMajor> > em2(m2);
    for(int i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }

    for(int i = 0; i < nts; i++){
        u0[i] = u0[i+nts] = 1.00/cosh(t[i]/2.0);
        help[i] = _real(u0[i]);
    }
    FILE* f = fopen("iso_out.3D", "w");
    fprintf(f, "X Y Z val\n");
    fftw_plan t2 = fftw_plan_dft_1d(nts, u0+nts, u0+nts, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan t1 = fftw_plan_dft_1d(nts, u0, u0, FFTW_BACKWARD, FFTW_ESTIMATE);
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
    FILE* fener = fopen("emax.out", "w");
   // values["a1"]->retrieve(&a1, 0);
    values["a1"]->retrieve(&a2, 0);
    values["a2"]->retrieve(&a3, 0);
    values["ap"]->retrieve(&ap, 0);
    a1=0;
      a2=.0496;
      a3=.4860;
      ap=1.541;
    controller* cont;
    values["cont"]->retrieve(&cont, 0);
    double xyz[3];
    int count=0;
    double ener;

    double norm, change_norm;
    for(int qq = 0; qq < num_steps; qq++){
        mscore = 0;
        mpulse=0;
        //   a3=0.4860;
        j1=rmat(a1).cast<comp>()*wq*rmat(-1*a1).cast<comp>();
        j2=rmat(a2).cast<comp>()*wq*rmat(-1*a2).cast<comp>();
        j3=rmat(a3).cast<comp>()*wh*rmat(-1*a3).cast<comp>();
        jp=rmat(ap).cast<comp>()*wp*rmat(-1*ap).cast<comp>();


        for(size_t i = 0; i < 50; i++){
            count++;
            //generate Jones matrices
            // copy current state to temp
            for(size_t j = 0; j < nts*2; j++){
                u1[j] = u0[j];
            }
            fft(t2, u0, u0, nts);
            fft(t2, u0+nts, u0+nts, nts);
            inter->integrate(rh, u0, 0, 1.5);
            ifft(t1, u0, u0, nts);
            ifft(t1, u0+nts, u0+nts, nts);
            dmap = j1*jp*j2*j3*dmap;
            norm = change_norm = 0;
            if(i){
                //find the change in norms
                for(size_t j = 0; j < nts; j++){
                    double fsq = _sqabs(u1[j]) + _sqabs(u1[j+nts]);
                    norm += fsq; 
                    double cfsq = sqrt(_sqabs(u0[j]) + _sqabs(u0[j+nts])) - sqrt(fsq);
                    change_norm += cfsq*cfsq;
                }
                change_norm = sqrt(change_norm/norm);  
                //         std::cout << change_norm << std::endl;
                if(change_norm < 1e-4){
                    break;
                }
            }
        }
        help[0] = sqrt(_sqabs(u0[0]) + _sqabs(u0[nts]));
        ener = help[0];
        for(size_t i = 1; i < nts-1; i++){
            help[i] = _sqabs(u0[i]) + _sqabs(u0[nts+i]);
            ener += 2*help[i];
            help[i]=sqrt(help[i]);
        }
        help[nts-1]= _sqabs(u0[nts-1]) + _sqabs(u0[2*nts-1]);
        ener += help[nts-1];
        help[nts-1]=sqrt(help[nts-1]);
        ener = sqrt(ener);
        double kurtosis_v = 1.0/(3+gsl_stats_kurtosis(help, 1, nts));
        if(kurtosis_v > mpulse){
            for(size_t j = 0; j < nts; j++){
                phold[j] = help[j];
            }
            mpulse = kurtosis_v;
        }
        if(fabs(ener*kurtosis_v) > mscore){
            for(size_t j = 0; j < nts; j++){
                mhold[j] = help[j];
            }
            mscore = fabs(ener*kurtosis_v);
        }
        std::cout << "Simulation ran " << count << " times, score was " << fabs(ener*kurtosis_v) <<  "\n";
        count = 0;
        fprintf(f, "%lf %lf %lf %lf\n", a1, a2, ap, ener*kurtosis_v);
        cont->control(0, 0);

        //printf( "%lf %lf %lf %lf\n", a1, a2, a3,ap);
    }
    for(int j = 0; j < nts; j++){
        fprintf(fmax, "%d, %lf\n", j, mhold[j]);
        fprintf(fpulse, "%d, %lf\n", j, phold[j]);
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
}
