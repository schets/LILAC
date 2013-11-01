#include <iostream>
#include "defs.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
using namespace std;
int main(int argc, char** argv){
    rhs* rh = new rhs_const(1, 2);
    integrator* inter = new rk4(5, 2);
    comp u0[2];
    u0[0] = 1;
    u0[1] = 2;
    inter->integrate(rh, u0, 0, 2);
    cout << creal(u0[0]) << " " << creal(u0[1]) << endl;
    delete inter;
    delete rh;
    return 0;
}
