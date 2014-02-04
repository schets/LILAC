#include "rhs/rhs.h"
#include "int_imp.h"
#include "integrator.h"


//use malloc instead of new since that allows use with fftw_malloc if so desired
rk4::~rk4(){
    free(f1);
    free(f2);
    free(f0);
    free(f3);
    free(u_calc);
}
/****************************************************************************80
 * comp *rk4vec ( double t0, int m, comp* u0, double dt, 
 rhs* func )


*/
//This has been modified to work with complex numbers
//and fit into the solver object structure.
//But the code mostly remains unchanged
int rk4::integrate(rhs* func, comp* restr u0, double t0, double tf)
    //****************************************************************************80
    //
    //  Purpose:
    //
    //    RK4VEC takes one Runge-Kutta step for a vector ODE.
    //
    //  Discussion:
    //
    //    It is assumed that an initial value problem, of the form
    //
    //      du/dt = f ( t, u )
    //      u(t0) = u0
    //
    //    is being solved.
    //
    //    If the user can supply current values of t, u, a stepsize dt, and a
    //    function to evaluate the derivative, this function can compute the
    //    fourth-order Runge Kutta estimate to the solution at time t+dt.
    //
    //  Licensing:
    //
    //    This code is distributed under the GNU LGPL license. 
    //
    //  Modified:
    //
    //    09 October 2013
    //
    //  Author
    //
    //    John Burkardt
    //
    //  Parameters:
    //
    //    Input, comp T0, the current time.
    //
    //    Input, int M, the spatial dimension.
    //
    //    Input, comp U0[M], the solution estimate at the current time.
    //
    //    Input, comp DT, the time step.
    //
    //    Input, comp *F ( comp T, int M, comp U[] ), a function which evaluates
    //    the derivative, or right hand side of the problem.
    //
    //    Output, comp RK4VEC[M], the fourth-order Runge-Kutta solution estimate
    //    at time T0+DT.
    //
{
    //get dt
    const size_t m = dimension;
    const double t_diff = tf-t0;
    const size_t steps = ceil(t_diff/stepsize);
    const double dt = t_diff/steps;
    //do steps integrations
    //each loop iteration is a runge_kutta timestep
    for(size_t j = 0; j < steps; j++){
        size_t i;
        double t1;
        double t2;
        double t3;
        //
        //  Get four sample values of the derivative.
        //
        func->dxdt(u0, f0, t0);

        t1 = t0 + dt / 2.0;
        for ( i = 0; i < m; i++ )
        {
            u_calc[i] = u0[i] + dt * f0[i] / 2.0;
        }
        func->dxdt(u_calc, f1, t1);
        t2 = t0 + dt / 2.0;
        for ( i = 0; i < m; i++ )
        {
            u_calc[i] = u0[i] + dt * f1[i] / 2.0;
        }

        func->dxdt(u_calc, f2, t2); 
        t3 = t0 + dt;
        for ( i = 0; i < m; i++ )
        {
            u_calc[i] = u0[i] + dt * f2[i];

        }

        func->dxdt(u_calc, f3, t3); 
        //
        //  Combine them to estimate the solution.
        //
        for ( i = 0; i < m; i++ )
        {
            u0[i] += (dt * ( f0[i]  + 2.0 * f1[i] + 2.0 * f2[i] + f3[i] ) / 6.0);
        }
        t0 += dt;
    }
    return 0;
}
void rk4::retrieve(void* p)const{}
void rk4::parse(std::string instr) {}
std::vector<std::string> rk4::dependencies()const{
    std::vector<std::string> tvec = integrator::dependencies();
    tvec.push_back("stepsize");
    return tvec;
}
std::string rk4::type()const{
    return "rk4";
}
void rk4::postprocess(std::map<std::string, item*>& dat){
    double sval;
    integrator::postprocess(dat);
    dat["stepsize"]->retrieve((void*)&stepsize, this);
    if(stepsize <= 0){
        err("stepsize is invalid, must be >= 0", "rk4::postprocess",
                "integrator/rk4.cpp", dat["stepsize"], FATAL_ERROR);
    }
    f0= (comp*)malloc(dimension*sizeof(comp));
    f1=(comp*)malloc(dimension*sizeof(comp));
    f2=(comp*)malloc(dimension*sizeof(comp)); 
    f3=(comp*)malloc(dimension*sizeof(comp));
    u_calc=(comp*)malloc(dimension*sizeof(comp));
}

