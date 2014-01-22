#include "engineimp.h"
#include "engine.h"
#include <iostream>
#include "utils/comp_funcs.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
#include <time.h>
void engineimp::run(){
    int ints;
    values["dimension"]->retrieve(&ints);
    size_t nts=(size_t)ints;
    nts/= 2;
    double t_int;
    values["t_int"]->retrieve(&t_int);
    if(!values.count("integrator")){
        err("integrator variable is not defined", "engineimp::run()",
                "parser/engine_run.cpp", FATAL_ERROR);
    }
    if(!values.count("rhs")){
        err("rhs variable is not defined", "engineimp::run()",
                "parser/engine_run.cpp", FATAL_ERROR);
    }
    integrator* inter= (integrator*)values["integrator"];
    rhs* rh = (rhs*)values["rhs"];
    comp* u0 = (comp*)malloc(2*nts*sizeof(comp));
    comp* u1 = (comp*)malloc(2*nts*sizeof(comp));
    double* t = (double*)malloc((nts)*sizeof(double)); 
    double dt =t_int*1.0/nts;

    for(int i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }
    for(int i = 0; i < nts; i++){
        u0[i] = u0[i+nts] = 1.00/cosh(t[i]/2.0);
    }
    fftw_plan t2 = fftw_plan_dft_1d(nts, u0+nts, u0+nts, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan t1 = fftw_plan_dft_1d(nts, u0, u0, FFTW_BACKWARD, FFTW_ESTIMATE);
    fft(t1, u0, u0, nts);
    fft(t2, u0+nts, u0+nts, nts);
    clock_t tval =clock();
    for(int i = 0; i < 00000; i++){
        rh->dxdt(u0, u1, 0);
        asm("");
    }
    rh->dxdt(u0, u1, 1);
    tval = clock()-tval;
    std::cout<<"time for rhs calls="<<tval<<std::endl;
    for(int i = 0; i < 1; i++){
              inter->integrate(rh, u0, 0, 15);
 //       rh->dxdt(u0, u1, 0);
        //     rh->dxdt(u1, u0, 0);

    }
    // ifft(t1, u0, u0, nts);
    //  ifft(t1, u0+nts, u0+nts, nts);
      comp* tmp = u1;
      u1=u0;
    ifft(t1, u0, u0, nts);
    ifft(t1, u0+nts, u0+nts, nts);
    for(int i = 0; i < nts*2; i++){
        std::cout<<_real(u0[i])<<"+"<< _imag(u0[i])<<"i\n";
        //     cout << _sqabs(u1[i]) << endl;
    }
    double ener = energy(u0, nts*2)*t_int/nts;
    std::cout << "Energy is: " << ener << std::endl;
     u1=tmp;
    //std::cout << ((double)clock() - tval)/CLOCKS_PER_SEC << std::endl;
    fftw_destroy_plan(t2);
    fftw_destroy_plan(t1);
    free(u0);
    free(u1);
    free(t);
}
