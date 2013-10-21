#include "rhs.h"
int rhs(double t, double* curx, double* dx, void* params){
    odeparams* p = params;
    return GSL_SUCCESS;
}
