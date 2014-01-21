#include <iostream>
#include <time.h>
#include "defs.h"
#include "comp_funcs.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
#include <parser/engine.h>
using namespace std;
int main(int argc, char** argv){
    /*size_t nts=5;
    double t_int = 60.0;
    rhs* rh = rhs::create("CNLS");
    integrator* inter = 0;//new rk4(1); 
    comp* u0 = (comp*)malloc(2*nts*sizeof(comp));
    comp* u1 = (comp*)malloc(2*nts*sizeof(comp));
    double* t = (double*)malloc((nts)*sizeof(double)); 
    double dt =t_int*1.0/nts;
    for(int i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }
    cout << "u0" << endl;
    for(int i = 0; i < nts; i++){
        u0[i] = u0[i+nts] = 1.00/cosh(t[i]/2.0);
    }
    fftw_plan t2 = fftw_plan_dft_1d(nts, u0+nts, u0+nts, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan t1 = fftw_plan_dft_1d(nts, u0, u0, FFTW_BACKWARD, FFTW_ESTIMATE);
    fft(t2, u0, u0, nts);
    fft(t2, u0+nts, u0+nts, nts);
    clock_t tval = clock();
    for(int i = 0; i < 1; i++){
   //      inter->integrate(rh, u0, 0, t_int);
          rh->dxdt(u0, u1, 0);
     //     rh->dxdt(u1, u0, 0);

    }
   // ifft(t1, u0, u0, nts);
  //  ifft(t1, u0+nts, u0+nts, nts);
  //  comp* tmp = u1;
  //  u1=u0;
    //fftw_norm(t1, u1, u1, nts);
    //fftw_norm(t1, u1+nts, u1+nts, nts);
    for(int i = 0; i < nts; i++){
        cout<<_real(u1[i])<<"+"<< _imag(u1[i])<<"i\n";
   //     cout << _sqabs(u1[i]) << endl;
    }
  //  u1=tmp;
    cout << ((double)clock() - tval)/CLOCKS_PER_SEC << endl;
    delete inter;
    delete rh;
    fftw_destroy_plan(t2);
    fftw_destroy_plan(t1);
    free(u0);
    free(u1);
    free(t);*/
    engine e("infile.in");
    e.run();
    //
    //This cleans up the fftw memory
    //not necessary since memory is freed on program exit
    //but makes finding memory leaks far far easier
    fftw_cleanup();
    return 0;
}
