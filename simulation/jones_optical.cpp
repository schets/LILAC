#include "jones_optical.h"
#include "comp_funcs.h"
#include <set>
#include <cstdlib>
#include <ctime>
double mom4(comp invals[], double* kvals, int len);
class _unique_name{
    std::set<std::string> names;
    public:
    std::string get_unique_name(std::string nbase){
        nbase.push_back('_');
        while(names.count(nbase)){
            nbase.push_back(1 | (char)(rand()));
        }
        names.insert(nbase);
        //ensure that the name will not be alread in the engine
        nbase.push_back('!');
        return nbase;
    }
    _unique_name(){
        srand(time(0));
    }
};
std::string get_unique_name(std::string base){
    static _unique_name nn;
    return nn.get_unique_name(base);
}

/*!
 * class to allow automatic updating of the matrices involved
 * It's hacky but there isn't a generic operator-creator made
 * for creating elements in the post processor of items yet
 */
class jones_matrix:public real8{

    public:
    double a1, a2, a3, ap;
    Eigen::Matrix<comp, 2, 2> wq, wh, wp, j1, j2, j3, jp;
    Eigen::Matrix<comp, 2, 2> mvals;
    Eigen::Matrix<comp, 2, 2> rmat(double alpha){
        Eigen::Matrix<comp, 2, 2> in;
        in(0, 0) = cos(alpha);
        in(0, 1) = -1*sin(alpha);
        in(1, 0)=sin(alpha);
        in(1, 1)=cos(alpha);
        return in;
    }

    virtual std::string type() const{
        return "jones_matrix";
    }
    virtual void update(){
        j1=rmat(a1)*wq*rmat(-1*a1);
        j2=rmat(a2)*wq*rmat(-1*a2);
        j3=rmat(a3)*wh*rmat(-1*a3);
        jp=rmat(ap)*wp*rmat(-1*ap);
        mvals = j1*jp*j2*j3;
    }
    jones_matrix(std::vector<variable*> avars, std::string n){
        setname(n);
        wq(1, 0) = wq (0, 1) = 0;
        wq(0, 0)=(comp)cexp(-1*I*3.14159*0.25);
        wq(1, 1)=(comp)cexp(1.0*I*3.14159*0.25);
        wh(1, 0)=wh(0, 1)=0;
        wh(0, 0)=-1.0*I;
        wh(1, 1)=1.0*I;
        wp(0, 1)=wp(1, 0)=0;
        wp(0, 0)=1;
        wp(1, 1)=0;
        avars[0]->retrieve(&a1, this);
        avars[1]->retrieve(&a2, this);
        avars[2]->retrieve(&a3, this);
        avars[3]->retrieve(&ap, this);
        update();
    }
};
std::vector<std::string> jones_optical::dependencies() const{
    std::string deps[] = {"num_jones_segments"};
    return appendvec(stable_ode::dependencies(), std::vector<std::string>(deps, deps+1));
}
void jones_optical::postprocess(std::map<std::string, item*>& invals){

    stable_ode::postprocess(invals);

    if(dimension%2){
        err("System jones_optical requires an even dimension", "jones_optical::postprocess",
                "system/jones_optical.cpp", FATAL_ERROR);
    }
    int num_segments;
    invals["num_jones_segments"]->retrieve(&num_segments, this);
    if(num_segments < 0){
        err("Number of jones segments must be greater than or equal to zero",
                "jones_optical::postprocess", "system/jones_optical.cpp", FATAL_ERROR);
    }
    help = (double*)al_malloc(sizeof(double)*dimension/2);
    t = (double*)al_malloc(sizeof(double)*dimension/2);
    kurtosis_help=(comp*)al_malloc(sizeof(comp)*dimension/2);
    phold=(double*)al_malloc(sizeof(double)*dimension/2);

    nts=dimension/2;
    fback = fftw_plan_dft_1d(nts, ucur, ucur, FFTW_BACKWARD, FFTW_ESTIMATE);
    ffor = fftw_plan_dft_1d(nts, ucur+nts, ucur+nts, FFTW_FORWARD, FFTW_ESTIMATE);
    double dt = 60.0/nts;
    for(int i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }


    for(int i = 0; i < nts; i++){
        ucur[i] = ucur[i+nts] = 1.00/cosh(t[i]/2.0);
        help[i] = _real(ucur[i]);
    }
    //generate variables for the jones matrices
    //create jones matrices
    std::string name_base = "jones_system_vars";
    std::string mat_base = "jones_system_matrices";

    for(int i = 0; i < num_segments; i++){
        std::vector<variable*> vv(4, (variable*)0);
        for(int j = 0; j < 4; j++){
            vv[j] = new variable();
            vv[j]->holder=holder;
            vv[j]->setname(get_unique_name(name_base));
            vv[j]->set(0);
            vv[j]->parse("0.1");
            invals[vv[j]->name()]=vv[j];
            if(j != 2){
                cont->addvar(vv[j]);
            }
        }

        jones_matrix* m = new jones_matrix(vv, get_unique_name(mat_base));
        invals[m->name()]=m;
        jones_matrices.push_back(m);
    }
}
/*!
 * FFT solutions since this is a spectral system
 */
void jones_optical::pre_integration_operations(){
    fft(ffor, ucur, ucur, nts);
    fft(ffor, ucur+nts, ucur+nts, nts);
}
void jones_optical::post_integration_operations(){
    Eigen::Map<Eigen::Matrix<comp, 2, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Aligned> dmap(ucur, 2, nts);
    ifft(fback, ucur, ucur, nts);
    ifft(fback, ucur+nts, ucur+nts, nts);
    //apply the jones matrices
    for(int i = 0; i < jones_matrices.size(); i++){
        dmap = jones_matrices[i]->mvals*dmap;
    }
}
double jones_optical::get_change(){
    double norm, change_norm;
    norm = change_norm = 0;
    for(size_t j = 0; j < nts; j++){
        double fsq = _sqabs(ulast[j]) + _sqabs(ulast[j+nts]);
        norm += fsq; 
        double cfsq = sqrt(_sqabs(ucur[j]) + _sqabs(ucur[j+nts])) - sqrt(fsq);
        change_norm += cfsq*cfsq;
    }
    if(norm < 1e-3){

        for(int i = 0; i < nts; i++){
            ucur[i] = ucur[i+nts] = 1.00/cosh(t[i]/2.0);
            help[i] = _real(ucur[i]);
        }
    }
    return sqrt(change_norm/norm);
}
double jones_optical::score(){
    double score = obj->score(ucur);
    if(score > best_score){
        best_score = score;
        ba1=jones_matrices[0]->a1;
        ba2=jones_matrices[0]->a2;
        bap=jones_matrices[0]->ap;
        for(int i = 0; i < nts; i++){
            phold[i] = sqrt(_sqabs(ucur[i]) + _sqabs(ucur[i+nts]));
        }
    }
    return score;
}
jones_optical::~jones_optical(){
    FILE* pmax = fopen("pmax_sys.out", "w");
    for(int i = 0; i < nts; i++){
        fprintf(pmax, "%d, %lf\n", i, phold[i]);
    }
    fclose(pmax);
    printf("%lf, %lf, %lf, %lf\n", ba1, ba2, bap, best_score);
    al_free(help);
    al_free(t);
    al_free(kurtosis_help);
    al_free(phold);
    fftw_destroy_plan(ffor);
    fftw_destroy_plan(fback);
}

std::string jones_optical::type() const{
    return "jones_optical";
}
