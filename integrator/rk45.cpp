#include "integrator.h"
#include "../utils/comp_funcs.h"
#include "../rhs/rhs.h"
#include "rk45.h"
void printar(comp* u0, comp* u1, int num){
    for(int i = 0; i < num; i++){
        std::cout<<_real(u0[i])<<"+"<< _imag(u0[i])<<"i, "<<_real(u1[i])<<"+"<<_imag(u1[i])<<"i\n";
    }
}
void printar(comp* u0, int num){
    for(int i = 0; i < num; i++){
        std::cout<<_real(u0[i])<<"+"<< _imag(u0[i])<<"i\n";
    }
}
void printar(double* u0, double* u1, int num){
    for(int i = 0; i < num; i++){
        std::cout<<u0[i]<<", "<<u1[i]<<std::endl;
    }
}
void printar(double* u0, int num){
    for(int i = 0; i < num; i++){
        std::cout<<u0[i]<<std::endl;
    }
}
inline void printv(comp c){
    std::cout << _real(c)<<"+"<<_imag(c)<<"i\n";
}
inline void printv(double c){
    std::cout << c << std::endl;
}
int rk45::integrate(rhs* func, comp* restr u0, double t0, double tf){
    size_t num_fail = 0;
    double dt = dt_init;
    double dtave=0;
    size_t steps=0;
    const double magic_power = 1.0/6; //found from reference file, reference p.91 Ascher and Petzold
    const double magic_mult = .8;//magic multiplying safety factor for determining the next timestep;

//    dorman prince integrator parameters
      const static double a[] = {0.2, 0.3, 0.8, 8.0/9, 1, 1};
      const static double b1 = 0.2;
      const static double b2[] = {3.0/40, 9.0/40};
      const static double b3[] = {44.0/45, -56.0/15, 32.0/9};
      const static double b4[] = {19372.0/6561, -25360.0/2187, 64448.0/6561, -212.0/729};
      const static double b5[] = {9017.0/3168, -355.0/33, 46732.0/5247, 49.0/176, -5103.0/18656};
      const static double b6[] = {35.0/384, 0, 500.0/1113, 125.0/192, -2187.0/6784, 11.0/84};
      const static double c4[] = {5179.0/57600, 0, 7571.0/16695, 393.0/640,
      -92097.0/339200, 187.0/2100, 1.0/40};
      const static double c5[] = {35.0/384, 0, 500.0/1113, 125.0/192, -2187.0/6784, 11.0/84, 0};
    /*
     * Fehlberg parameters, usable if one wants
     * Fehlberg minimizes the 4th order error, so is less desireable
    const double a[] = {1.0/4, 3.0/8, 12.0/13, 1.0, 1.0/2};
    const double b1 = 0.25;
    const double b2[] = {3.0/32, 9.0/32};
    const double b3[] = {1932.0/2197, -7200.0/2197, 7296.0/2197};
    const double b4[] = {439.0/216, -8.0, 3680.0/513, -845.0/4104};
    const double b5[] = {-8.0/27, 2.0, -3544.0/2565, 1859.0/4104, -11.0/40};
    const double c4[] = {25.0/216, 0, 1408.0/2565, 2197.0/4104, -1.0/5, 0};
    const double c5[] = {16.0/135, 0, 6656.0/12825, 28561.0/56430, -9.0/50, 2.0/55};*/
    double taui;
    taui = 0;
    double tcur=t0;
    double tauv = 0;
    for(size_t i = 0; i < dimension; i++){
        double taum = _sqabs(u0[i]);
        if(taum > tau){
            taui = taum;
        }
    }
    tauv = std::sqrt(taui);
    tauv *= relerr;
    comp* restr tmp, * restr swp, * restr u0hld;//this is used for freeing the memory later
    comp* restr tmp6, * restr tmpc;
    tmp6 = f6;
    tmpc = u_calc;
    u0hld=u0;
    //used to avoid memory problems later on
    //allows for easy switching of pointers to avoid memory copies

    tmp=f0;
    //removes the need to check if the pointers should be swapped or not
    if((tcur + dt) > tf){
        dt = tf - tcur;
        err("t0-tf <= dt, consider decreasing the initial timestep. Otherwise mileage may vary",
                "rk45::integrate", "integrator/rh45.cpp", (item*)func, WARNING);
    }
    int tries=0;

    //while this seems odd, it helps streamline the inner integrator loop
    func->dxdt(u0, f6, tcur);
    while(tcur < tf){
        //this function=lol at compiler loop unrolling
        if((tcur + dt) > tf){
            break;
        }
        tries++;
        swp=f6;
        f6=f0;
        f0=swp;
        double ts = tcur;
        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + b1*f0[i]*dt;
        }
        tcur = ts + a[0]*dt;
        func->dxdt(u_calc, f1, tcur); 
        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b2[0]*f0[i] + b2[1] * f1[i]);
        }
        tcur = ts + a[1]*dt;
        func->dxdt(u_calc, f2, tcur);
        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b3[0]*f0[i] + b3[1] * f1[i] + b3[2]*f2[i]);
        }
        tcur = ts + a[2]*dt;
        func->dxdt(u_calc, f3, tcur);
        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b4[0]*f0[i] + b4[1] * f1[i] + b4[2]*f2[i] + b4[3]*f3[i]);
        }
        tcur = ts + a[3]*dt;
        func->dxdt(u_calc, f4, tcur);
        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b5[0]*f0[i] + b5[1] * f1[i] + b5[2]*f2[i] + 
                    b5[3]*f3[i] + b5[4]*f4[i]);
        }
        tcur = ts + a[4]*dt;
        func->dxdt(u_calc, f5, tcur);   
        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(b6[0]*f0[i] + b6[1] * f1[i] + b6[2]*f2[i] + 
                    b6[3]*f3[i] + b6[4]*f4[i] + b6[5]*f5[i]);
        }
        tcur = ts + a[5]*dt;
        func->dxdt(u_calc, f6, tcur);   
        tcur = ts;
        //calculate the magnitude of the error, and the fourth/5th order arrays
        //The references that I found are for real values
        //so I substitute the magnitude of complex differences for the absolute real difference
        //not sure if this is the way to go, but sounds about right
        //internally, I do calculations with the squares to avoid calculations of sqrt
        //This is done with the inf norm
        double delta=1E-12;
        for(size_t i = 0; i < dimension; i++){
            u_calc[i] = u0[i] + dt*(c5[0]*f0[i] + c5[1] * f1[i] + c5[2] * f2[i] + c5[3] * f3[i] + 
                    c5[4]*f4[i] + c5[5]*f5[i] + c5[6]*f6[i]);

            u_calc2[i] = _sqabs(u0[i] + dt*(c4[0]*f0[i] + c4[1] * f1[i] + c4[2] * f2[i] + c4[3] * f3[i] + 
                    c4[4]*f4[i] + c4[5]*f5[i] + c4[6]*f6[i]) - u_calc[i]);
        }
            
        //seperate these two loops to allow vectorization of the first}
        for(size_t i = 0; i < dimension; i++){
            double deltam = u_calc2[i];//square of absolute error
            if(deltam>delta){
                delta=deltam;
            }
        }
        delta = std::sqrt(delta);
        //estimated optimal dt, smallest of the two options to ensure accuracy
        double dt_last = dt;
        dt =dt*magic_mult*std::pow(tauv/delta, magic_power);
        if(dt < dt_min){
            num_fail++;
            if(num_fail > 10){
                err("Estimated timestep is smaller than minimum timestep too many times, exiting",
                        "rk45::integrate", "integrator/rk45.cpp", (item*)func, WARNING);
                f0=tmp;
                f6 = tmp6;
                u_calc=tmpc;
                return -1;
            }
            //std::cout << "dt= " << dt << ", dt_min=" << dt_min<<std::endl;
            dt = dt_min;
        }
        else{
            num_fail = 0;
        }
        dt = std::min(dt, dt_max);
        if(delta>=tauv){
            swp=f6;
            f6=f0;
            f0=swp;
            continue;
        }
        tcur += dt_last;
        if((tcur + dt) > tf){
            dt = tf-tcur;
        }
        // #define give_out
#ifdef give_out
        std::cout << "delta="<<delta<<"\ntauv="<<tauv<<"\ntauv/delta="<<(tauv/delta)<<
            "\nmul_fac="<<magic_mult*std::pow(tauv/delta, magic_power)<<"\ndt_last="<<dt_last<<"\ndt="<<dt<<"\ntcur="<<tcur<<
            "\ntries="<<tries<<std::endl;
        /*        std::cout<<"\nf0=\n";
                  printar(f0, dimension);
                  std::cout<<"f1=\n";
                  printar(f1, dimension);
                  std::cout<<"f2=\n";
                  printar(f2, dimension);
                  std::cout<<"f3=\n";
                  printar(f3, dimension);
                  std::cout<<"f4=\n";
                  printar(f4, dimension);
                  std::cout<<"f5=\n";
                  printar(f5, dimension);
                  std::cout<<"f6=\n";
        //       printar(u_calc, dimension);
        */
        std::cin.get();

#endif

        for(size_t i = 0; i < dimension; i++){
            double val = _sqabs(u_calc[i]);
            if(val > taui){
                taui = val;
            }
        }
        steps++;
        dtave+=dt_last;
        tauv=std::sqrt(taui);
        tauv *= relerr;
        tries=0;
        //e
        //rror is acceptable, continue on
        swp = u_calc;
        u_calc = u0;
        u0 = swp;
        //instead of copying u_calc to u_0 swap pointers
}
//check if a memory copy needs to be done on u0
//u0 currently points to the most recent update
//if u0 does not point to where it originally did, re-copy the memory over
if(u0 != u0hld){
    for(size_t i = 0; i < dimension; i++){
        u0hld[i] = u0[i];
    }
}
//u0, or at least that original address, now holds the final integration values
f0=tmp;
f6 = tmp6;
u_calc=tmpc;
//#define PRINT_TIME
#ifdef PRINT_TIME
std::cout << "steps takes was " << steps << ", average step size was " << dtave/steps<< "\n";
#endif
return 0;
}
rk45::~rk45(){
    al_free(f0);
    al_free(u_calc2);
}
std::vector<std::string> rk45::dependencies() const{
    std::string deps[] = {"dt_init", "dt_min", "dt_max", "relerr", "abserr"};
    return appendvec(std::vector<std::string>(deps, deps+5), integrator::dependencies());
}
std::string rk45::type() const{
    return "rk45";
}
void rk45::postprocess(std::map<std::string, item*>& dat){
    integrator::postprocess(dat);
    dat["dt_init"]->retrieve((void*)&dt_init);
    if(dt_init <= 0){
        err("dt_init is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["dt_init"], FATAL_ERROR);
    }
    dat["dt_min"]->retrieve((void*)&dt_min);
    if(dt_min <= 0){
        err("dt_min is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["dt_min"], FATAL_ERROR);
    }
    dat["dt_max"]->retrieve((void*)&dt_max);
    if(dt_max <= 0){
        err("dt_max is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["dt_max"], FATAL_ERROR);
    }
    dat["relerr"]->retrieve((void*)&relerr);
    if(relerr <= 0){
        err("relerr is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["relerr"], FATAL_ERROR);
    }
    dat["abserr"]->retrieve((void*)&abserr);
    if(abserr <= 0){
        err("abserr is invalid, must be >= 0", "rk45::postprocess",
                "integrator/rk45.cpp", dat["abserr"], FATAL_ERROR);
    }
    f0 = (comp*) al_malloc(8*sizeof(comp)*dimension);
    f1=f0+dimension;
    f2=f1+dimension;
    f3=f2+dimension;
    f4=f3+dimension;
    f5=f4+dimension;
    f6=f5+dimension;
    u_calc=f6+dimension;
    u_calc2 = (double*) al_malloc(dimension*sizeof(double));
}
void rk45::parse(std::string inval){
};
void rk45::print() const{
};
