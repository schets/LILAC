#include "pulse_check.h"
#include <math.h>
#include <gsl/gsl_multimin.h>
#include <stdio.h>
/*!
 * This constant is the exponent that the score  is subjected to
 * before being returned. A higher exponent punishes non-perfect sech curves
 * more strongly
 */
const static double mul_pow = 1;
typedef struct {
    double* tv;
    double* uvals;
    size_t n;
} mindata;

double minfunc(const gsl_vector* x, void* _params){
    mindata* p = _params;
    double a = gsl_vector_get(x, 0);
    double b = gsl_vector_get(x, 1);
    double c = gsl_vector_get(x, 2);
    double d = gsl_vector_get(x, 3);
    double* tv = p->tv;
    double* uvals = p->uvals;
    //calculate model fit
    //not optimal way to do things but this is such a
    //small portion of the computation it barely even matters
    size_t i;
    double curdiff = 0;
    for(i = 0; i < p->n; i++){
        double curval = c+ a/cosh(d*(tv[i] -  b));
        curval -= uvals[i];
        curdiff += curval * curval;
    }
    return sqrt(fabs(curdiff));
}
double pulse_check(double* invals, double* tvals, size_t numvals){
    size_t iter = 0;
    int status;
    const gsl_multimin_fminimizer_type* T = gsl_multimin_fminimizer_nmsimplex;
    gsl_multimin_fminimizer *s;
    const size_t n = numvals;
    const size_t p = 4;
    mindata dat = {tvals, invals, numvals};
    double x_init[4] = {0.5, 1.0, 0, 1.0};
    double curmin = 1;
    gsl_vector *x = gsl_vector_alloc(p);
    gsl_vector_set(x, 0, 0.5);
    gsl_vector_set(x, 1, 1.0);
    gsl_vector_set(x, 2, 0.0);
    gsl_vector_set(x, 3, 1.0);
    gsl_multimin_function f;
    f.f = &minfunc;
    f.params=&dat;
    f.n = p;
    gsl_vector* ss = gsl_vector_alloc(p);
    gsl_vector_set_all(ss, .10);
    s=gsl_multimin_fminimizer_alloc(T, p);
    gsl_multimin_fminimizer_set(s, &f, x, ss);

    //iterate the solver until it has converged
    do
    {
        iter++;
        status = gsl_multimin_fminimizer_iterate (s);
        curmin = gsl_multimin_fminimizer_minimum(s);
        if (status)
            break;

        status = gsl_multimin_test_size (gsl_multimin_fminimizer_size(s), 1e-4);
    }
    while (status == GSL_CONTINUE && iter < 500);
    double norm, cnorm = 0;
    gsl_vector_free(x);
    gsl_vector_free(ss);
    gsl_multimin_fminimizer_free(s);
    //find norm of the error, norm of the solution
    size_t j;
    for(j = 0; j < n; j++){
        double iv = invals[j] * invals[j];
        norm += iv;
    }
    norm = sqrt(norm);
  //  printf("norm: %lf\n", norm);
    double relerr = curmin;
    return 1.0/relerr;
    //return the relative error
 /*   double a = gsl_vector_get(x, 0);
    double b = gsl_vector_get(x, 1);
    double c = gsl_vector_get(x, 2);*/
  //  double tscore =  pow(relerr/100, -1 * mul_pow);
    //return (tscore)/(1+tscore);
}
