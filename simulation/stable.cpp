#include "stable.h"
#include "objective/objective.h"
#include "comp_funcs.h"
/*!
 * This function iterates the system forwards in time until it reaches a stable state
 * or a certain number of iterations, 
 * and then returns the objective function of that state.
 */
double stable::simulate(){
    bad_res=0;
    for (round = 0; round < max_iterations; round++){

        this->iterate_system();

        if(bad_res<0){
            return 0;
        }
        if(round){
            double chan = this->get_change();
            if(chan < change_threshold){
                break;
            }
        }

    }
    double v = this->score();
    if(1 || !num_gone || !(num_gone%1)){
        printf("System:%d, test# %d,  took %d iterations, score was %e\n",
                cont->index, num_gone, round, v);
    }

    num_gone++;
    return v;
}
std::vector<std::string> stable::dependencies() const{
    std::string deps[] = {"change_threshold", "max_iterations"};
    return appendvec(simulation::dependencies(), std::vector<std::string>(deps, deps+2));
}

void stable::postprocess(std::map<std::string, item*>& invals){
    num_gone=0;
    simulation::postprocess(invals);
    invals["change_threshold"]->retrieve(&change_threshold, this);
    if(change_threshold < 0){
        err("Input parameter change_threshold must be greater than or equal to zero",
                "stable::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
    invals["max_iterations"]->retrieve(&max_iterations, this);
    if(max_iterations <= 0){
        err("Input parameter max_iterations must be greater than or equal to zero",
                "stable::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
}

//stable_ode functions

/*!
 * This is the function that iterates the ODE system forwards.
 * It applies the operator pre_integration_operations, then integrates
 * from tcur to tcur+tlen, and then applies the operator post_integration_operations.
 * \sa stable::simulate
 */
void stable_ode::iterate_system(){
    for(size_t i = 0; i < dimension; i++){
        ulast[i] = ucur[i];
    }
    this->pre_integration_operations(); 
    int res = inter->integrate(rh, ucur, tcur, tcur+int_len);
    tcur += int_len;
    this->post_integration_operations();
    if(res < 0){
        bad_res=1;
    }
}

void stable_ode::postprocess(std::map<std::string, item*>& invals){
    stable::postprocess(invals);
    invals["rhs"]->retrieve(&rh, this);
    invals["integrator"]->retrieve(&inter, this);
    invals["t0"]->retrieve(&t0, this);
    tcur = t0;
    invals["int_len"]->retrieve(&int_len, this);
    const double eps_val = 1e-12;
    if(int_len < eps_val){
        err("Variable int_len is too small, int_len must be greater than 1e-12",
                "stable_ode::postprocess", "system/stable.cpp", FATAL_ERROR);
    }
    ucur = (comp*)al_malloc(2*dimension*sizeof(comp));
    ulast=ucur+dimension;
}

stable_ode::~stable_ode(){
    al_free(ucur);
}
void stable_ode::pre_integration_operations(){
}
void stable_ode::post_integration_operations(){
}
std::string stable_ode::type() const{
    return "stable_ode";
}
std::vector<std::string> stable_ode::dependencies() const{
    std::string deps[] = {"rhs", "integrator", "t0", "int_len"};
    return appendvec(std::vector<std::string>(deps, deps+4), stable::dependencies());
}
double stable_ode::score(){
    return obj->score(ucur);
}

//stable_spectral_pde_1d functions

std::string stable_spectral_pde_1d::type() const{
    return "stable_spectral_pde_1d";
}

std::vector<std::string> stable_spectral_pde_1d::dependencies() const{
    std::string deps[] = {"num_pulses"};
    return appendvec(std::vector<std::string>(deps, deps+1), stable_ode::dependencies());
};

void stable_spectral_pde_1d::postprocess(std::map<std::string, item*>& invals){
    stable_ode::postprocess(invals);
    int _num;
    invals["num_pulses"]->retrieve(&_num, this);
    if(_num <= 0){
        err("Number of pulses must be greater than zero", "stable_spectral_pde_1d::postprocess",
                "simulation/stable.cpp", FATAL_ERROR);
    }
    num_pulses=_num;
    if(dimension%num_pulses){
        err("Dimension must be divisible by the number of pulses",  "stable_spectral_pde_1d::postprocess",
                "simulation/stable.cpp", FATAL_ERROR);
    }
    nts=dimension/num_pulses;

    ffor = fftw_plan_dft_1d(nts, ucur, ucur, FFTW_FORWARD, FFTW_ESTIMATE);
    fback = fftw_plan_dft_1d(nts, ucur, ucur, FFTW_BACKWARD, FFTW_ESTIMATE);
    t=new double[nts];
    double dt=60.0/nts;
    for(int i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }
    help = new double[nts];
    for(int i = 0; i < nts; i++){
        ucur[i] = ucur[i+nts] = 1.00/cosh(t[i]/2.0);
        help[i] = _real(ucur[i]);
    }
}
void stable_spectral_pde_1d::pre_integration_operations(){
    if(round==0){
        for(int i = 0; i < nts; i++){
            ucur[i] = ucur[i+nts] = 1.00/cosh(t[i]/2.0);
        }
    }
    pre_fft_operations();
    for(size_t i = 0; i < num_pulses; i++){
        fft(ffor, ucur+i*nts, ucur+i*nts, nts);
    }
    post_fft_operations();
}
void stable_spectral_pde_1d::post_integration_operations(){
    pre_ifft_operations();

    for(size_t i = 0; i < num_pulses; i++){
        ifft(fback, ucur+i*nts, ucur+i*nts, nts);
    }
    post_ifft_operations();
}
void stable_spectral_pde_1d::pre_fft_operations(){};
void stable_spectral_pde_1d::post_fft_operations(){};
void stable_spectral_pde_1d::pre_ifft_operations(){};
void stable_spectral_pde_1d::post_ifft_operations(){};
stable_spectral_pde_1d::~stable_spectral_pde_1d()
{
    delete[] t;
    delete[] help;
    fftw_destroy_plan(ffor);
    fftw_destroy_plan(fback);
}

