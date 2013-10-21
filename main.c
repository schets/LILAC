#include "defs.h"
#include "rhs.h"
//matrix[i,j] = matrix[i + j*width]
inline void R_matrix(real alpha, real* mat){
    mat[0] = cos(alpha);
    mat[1] = -1*sin(alpha);
    mat[2] = sin(alpha);
    mat[3] = cos(alpha);
}
inline void R_matrix_c(real alpha, comp* mat){
    mat[0] = ccos(alpha);
    mat[1] = -1*csin(alpha);
    mat[2] = csin(alpha);
    mat[3] = ccos(alpha); 
}
//no point in blas for 2*2 matrix
//gcc will make optimal I hope, should all be in cache anyways
inline void matmul2d(real* a, real* b, real* c){
    c[0] = a[0] * b[0] + a[1] * b[2];
    c[1] = a[0] * b[1] + a[1] * b[3];
    c[2] = a[2] * b[0] + a[3] * b[2];
    c[3] = a[2] * b[1] + a[3] * b[3];
}
//result stored in b in this case
inline void matmul2d_in(real* a, real* b){
    real c[4];
    c[0] = a[0] * b[0] + a[1] * b[2];
    c[1] = a[0] * b[1] + a[1] * b[3];
    c[2] = a[2] * b[0] + a[3] * b[2];
    c[3] = a[2] * b[1] + a[3] * b[3];
    b[0] = c[0];
    b[1] = c[1];
    b[2] = c[2];
    b[3] = c[3];
}
inline void matmul2d_c(comp* a, comp* b, comp* c){
    c[0] = a[0] * b[0] + a[1] * b[2];
    c[1] = a[0] * b[1] + a[1] * b[3];
    c[2] = a[2] * b[0] + a[3] * b[2];
    c[3] = a[2] * b[1] + a[3] * b[3];
}
//result stored in b in this case
inline void matmul2d_in_c(comp* a, comp* b){
    real c[4];
    c[0] = a[0] * b[0] + a[1] * b[2];
    c[1] = a[0] * b[1] + a[1] * b[3];
    c[2] = a[2] * b[0] + a[3] * b[2];
    c[3] = a[2] * b[1] + a[3] * b[3];
    b[0] = c[0];
    b[1] = c[1];
    b[2] = c[2];
    b[3] = c[3];
}
inline void normalize(comp* val){
    val = __builtin_assume_aligned(val, 16);
    for(iter i = 0; i < NUM_TIME_STEPS; i++){
        val[i] /= NUM_TIME_STEPS;
    }
}
void printmat(real* c){
    printf("%.3f %.3f\n%.3f %.3f\n", c[0], c[1], c[2], c[3]);
}
void printmat_c(comp* c){
    printf("%.3f+%.3fi %.3f+%.3fi\n%.3f+%.3fi %.3f+%.3fi\n",
            creal(c[0]), cimag(c[0]), creal(c[1]), cimag(c[1]),
            creal(c[2]), cimag(c[2]), creal(c[3]), cimag(c[3]));
}
void printcomp(comp c){
    printf("%.3f + %.3fi", creal(c), cimag(c));
}
int main(int argc, char** argv){
    clock_t tt;
    real t[NUM_TIME_STEPS+1];
    //input variables
    real alpha1, alpha2, alpha3, alpha_p;
    //standard values from matlab code
    alpha1=-.4668;
    alpha2=0.0496;
    alpha3=0.4860;
    alpha_p=1.5410;
    //constructing the jones matrices
    comp W_q[4]={cexp(-1*I*PI/4), 0, 0, exp(I*PI/4)};
    comp W_h[4]={-1*I, 0, 0, I};
    comp W_p[4]={1, 0, 0, 0};
    comp J_1[4], J_2[4], J_3[4], J_p[4], mul_hold[4], r_mat[4];
    //build the jones matrices
    //jones matrix 1
    R_matrix_c(-1*alpha1, r_mat);
    matmul2d_c(W_q, r_mat, mul_hold);
    R_matrix_c(alpha1, r_mat);
    matmul2d_c(r_mat, mul_hold, J_1);
    //matrix 2
    R_matrix_c(-1*alpha2, r_mat);
    matmul2d_c(W_q, r_mat, mul_hold);
    R_matrix_c(alpha2, r_mat);
    matmul2d_c(r_mat, mul_hold, J_2);
    //matrix 3
    R_matrix_c(-1*alpha3, r_mat);
    matmul2d_c(W_h, r_mat, mul_hold);
    R_matrix_c(alpha3, r_mat);
    matmul2d_c(r_mat, mul_hold, J_3);   
    //matrix p
    R_matrix_c(-1*alpha_p, r_mat);
    matmul2d_c(W_p, r_mat, mul_hold);
    R_matrix_c(alpha_p, r_mat);
    matmul2d_c(r_mat, mul_hold, J_p);
    //jones matrices have been constructed
    //build the t array
    real dt =LENGTH_T*1.0/NUM_TIME_STEPS;
    for(int i = 0; i < NUM_TIME_STEPS+1; i++){
        t[i] = dt*((real)i-NUM_TIME_STEPS/2.0);
    }
    nl;
    //build initial condition 
    //use fftw_malloc to ensure proper memory alignment
    comp* U_int1 = malloc(sizeof(comp)*NUM_TIME_STEPS);
    comp* U_int2 = malloc(sizeof(comp)*NUM_TIME_STEPS);
    comp* U_int1_f = malloc(sizeof(comp)*NUM_TIME_STEPS);
    comp* U_int2_f = malloc(sizeof(comp)*NUM_TIME_STEPS);
    for(int i = 0; i < NUM_TIME_STEPS; i++){
        U_int1[i] = U_int2[i] = 1.0/cosh(t[i]/2.0);
    }
    fftw_plan p_for1, p_for2, p_back1, p_back2;
    p_for1=fftw_plan_dft_1d(NUM_TIME_STEPS, U_int1, U_int1_f, FFTW_FORWARD, FFTW_MEASURE);
    p_for2=fftw_plan_dft_1d(NUM_TIME_STEPS, U_int2, U_int2_f, FFTW_FORWARD, FFTW_MEASURE);
    p_back1=fftw_plan_dft_1d(NUM_TIME_STEPS, U_int1_f, U_int1, FFTW_BACKWARD, FFTW_MEASURE);
    p_back2=fftw_plan_dft_1d(NUM_TIME_STEPS, U_int2_f, U_int2, FFTW_BACKWARD, FFTW_MEASURE);
    //build array of spectral values
    //fftw does method similar to matlab where the positive frequencies are stored in the first half
    //and the negative frequencies are stored in the second half
    real* k = malloc(NUM_TIME_STEPS*sizeof(real));
    real mulval=(2.0*PI/LENGTH_T)*(NUM_TIME_STEPS/2.0);
    printf("positive k values\n");
    for(int i=0; i<NUM_TIME_STEPS/2; i++){
        k[i] = mulval * i;
        printf("%.2f, ", k[i]);
    }
    printf("negative k values (in same order as array)\n");
    for(int i=NUM_TIME_STEPS/2; i<NUM_TIME_STEPS; i++){
        k[i] = mulval * (i-NUM_TIME_STEPS);
        printf("%.2f, ", k[i]);
    }

    nl;
    nl;
    tt = clock();//used to measure the time the program takes
    //build fourier transform initial conditions
    fftw_execute(p_for1);
    fftw_execute(p_for2);
    //the above is not very optimal. However, this is a tiny portion of the computations and
    //the fft and hopefully ODE integration will be well optimized by this point
    //I'm not sure how well the gsl ODE solver are optimized for fixed time step problems
    //ODEPACK is much better for stiff problems, but is in fortran and may introduce extra overhead
    //for a fixed time step problem by dealing with the jacobian and stuff. If anyone knows of
    //decent benchmarks for the GSL fixed time step, or if there is a known good ODE integrator
    //for this type of problem let me know
    //change_norm is 1e1000 in matlab, but can be made lower without negative effect
    real change_norm=1;
    int j = 1; 
    real norms[50];
    while(j<=maxTrips && change_norm > 1.0e-3){
        j++;
    }
    printf("Number of round trips: %d\n", j); 
    //free memory
    tt = clock() - tt;
    printf("Total elapsed time was %lf seconds\n", (real)((real)tt)/CLOCKS_PER_SEC);
    free(U_int1);
    free(U_int2);
    free(U_int1_f);
    free(U_int2_f);
    free(k);
    fftw_destroy_plan(p_for1);
    fftw_destroy_plan(p_for2);
    fftw_destroy_plan(p_back1);
    fftw_destroy_plan(p_back2);
    return 0;
}
