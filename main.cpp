#include <iostream>
#include "defs.h"
#include "comp_funcs.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
using namespace std;
int main(int argc, char** argv){
    size_t nts=256;
    rhs* rh = new rhs_CNLS(2*nts, 1.73, 4.23, 60.00/nts, 60, nts);
    integrator* inter = new rk4(5000, 2*nts);
    comp* u0 = (comp*)malloc(2*nts*sizeof(comp));
    double* t = (double*)malloc((nts+1)*sizeof(double)); 
    double dt =60*1.0/nts;
    for(int i = 0; i < nts+1; i++){
        t[i] = dt*((double)i-nts/2.0);
    }
    for(int i = 0; i < nts; i++){
        u0[i] = u0[i+nts] = 1.00/ccosh(t[i]/2.0);
    }

    fftw_plan t2 = fftw_plan_dft_1d(nts, u0+nts, u0+nts, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan t1 = fftw_plan_dft_1d(nts, u0, u0, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(t2);
    fftw_execute(t1);
    for(int i = 0; i < 1; i++){
        inter->integrate(rh, u0, 0, 60);
    }
    for(int i = 0; i < nts; i++){
        cout << _real(u0[i]) << "+" << _imag(u0[i]) << "i, " <<
                _real(u0[i+nts]) << "+" << _imag(u0[i+nts]) << "i" << endl;
    }
    delete inter;
    delete rh;
    fftw_destroy_plan(t2);
    fftw_destroy_plan(t1);
    free(u0);
    free(t);
    return 0;
}
