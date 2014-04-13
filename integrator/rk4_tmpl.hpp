#include "integrator.h"
#include "rk4.h"
#include "item_heads.hpp"
template<class T>
class rk4_tmpl:public rk4 {
    protected:
    T* restr f0, * restr f1, * restr f2, * restr f3;
    T* restr u_calc;
    rhs* func;
    double stepsize;
    public:
    virtual const std::type_info& vtype() const;
    void postprocess(input& dat);
    std::string type() const;
    int integrate(void* restr u, double t0, double tf);
    virtual ~rk4_tmpl();
};
template<class T>
const std::type_info& rk4_tmpl<T>::vtype()const{
    return typeid(T);
}
//use malloc instead of new since that allows use with fftw_malloc if so desired
template<class T>
rk4_tmpl<T>::~rk4_tmpl(){
 }
/****************************************************************************80
 * comp *rk4vec ( double t0, int m, comp* u0, double dt, 
 rhs* func )


*/
//This has been modified to work with complex numbers
//and fit into the solver object structure.
//But the code mostly remains unchanged
template<class T>
int rk4_tmpl<T>::integrate(void* restr _u0, double t0, double tf)
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
    T* restr u0 = (T* restr)_u0;
    ALIGNED(f0);
    ALIGNED(f1);
    ALIGNED(f2);
    ALIGNED(f3);
    ALIGNED(u0);
    ALIGNED(u_calc);
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
template<class T>
std::string rk4_tmpl<T>::type()const{
    return std::string("rk4_tmpl<") + this->vname() + ">";
}
template<class T>
void rk4_tmpl<T>::postprocess(input& dat){
    integrator::postprocess(dat);
    if(!rh_val->compare<T>()){
        err("Bad rhs type passed to rk4 integrator", "rk4_tmpl::postprocess",
                "integrator/rk4_tmpl.h", FATAL_ERROR);
    }
    this->add_as_parent(rh_val);
    retrieve(stepsize, dat["stepsize"], this);
    if(stepsize <= 0){
        err("stepsize is invalid, must be >= 0", "rk4_tmpl::postprocess",
                "integrator/rk4.hpp", dat["stepsize"].get(), FATAL_ERROR);
    }
    memp.create(dimension, &f0, &f1, &f2, &f3, &u_calc);
}
