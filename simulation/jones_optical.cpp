#include "jones_optical.h"
#include "comp_funcs.h"
#include <set>
#include <cstdlib>
#include <ctime>
#include <cmath>
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
double gaussrand()
{
    static double U, V;
    static int phase = 0;
    double Z;

    if(phase == 0) {
        U = (rand() + 1.) / (RAND_MAX + 2.);
        V = rand() / (RAND_MAX + 1.);
        Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
    } else
        Z = sqrt(-2 * log(U)) * cos(2 * PI * V);

    phase = 1 - phase;

    return Z;
}
void noise(comp* inval, double norm, size_t len){
    for(size_t i = 0; i < len; i++){
        inval[i] = gaussrand() + I*gaussrand();
    }
    double n = 0;
    for(size_t i = 0; i < len; i++){
        n += _sqabs(inval[i]);
    }
    n = sqrt(n);
    double mval = norm/n;
    for(size_t i = 0; i < len; i++){
        inval[i] *= mval;
    }
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
            /*  a1=2.6562;
                a2=1.274936;
                a3=5.003272;
                ap=2.871042;
                a1=a2 = 0;
                a2=.12;
                a3=1.2;
                ap=3.4;*/
            j1=rmat(a1)*wq*rmat(-1*a1);
            j2=rmat(a2)*wq*rmat(-1*a2);
            j3=rmat(a3)*wh*rmat(-1*a3);
            jp=rmat(ap)*wp*rmat(-1*ap);
            mvals = j1*jp*j2*j3;
        }
        jones_matrix(std::vector<variable*> avars, std::string n){
            setname(n);
            wq(1, 0) = wq (0, 1) = 0;
            wq(0, 0)=std::exp(-1.0*I*3.14159*0.25);
            wq(1, 1)=std::exp(1.0*I*3.14159*0.25);
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
    std::string deps[] = {"num_jones_segments", "jones_int_dist"};
    return appendvec(stable_spectral_pde_1d_tmpl<comp>::dependencies(), std::vector<std::string>(deps, deps+2));
}
void jones_optical::postprocess(std::map<std::string, item*>& invals){
#ifdef gen_t_dat
    func_dat=fopen("python/grad_data2.out", "w");
    func_score = fopen("python/score_data2.out", "w");
#else
    func_dat=fopen("python/grad_data.out", "w");
    func_score = fopen("python/score_data.out", "w");
#endif
    stable_spectral_pde_1d_tmpl<comp>::postprocess(invals);

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
    invals["jones_int_dist"]->retrieve(&jones_int_dist, this);
    if(jones_int_dist<0){
        err("The distance between jones segments, jones_int_dist,  must be greater than or equal to zero",
                "jones_optical::postprocess", "system/jones_optical.cpp", FATAL_ERROR);
    }

    help = (double*)al_malloc(sizeof(double)*nts);
    t = (double*)al_malloc(sizeof(double)*nts);
    kurtosis_help=(comp*)al_malloc(sizeof(comp)*nts);
    phold=(double*)al_malloc(sizeof(double)*nts);
    nvec1= (comp*)al_malloc(dimension*sizeof(comp));
    nvec2= (double*)al_malloc(nts*sizeof(double));
    double dt = 60.0/nts;
    noise(ucur, 0.2, dimension); 
    for(int i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }
    for(int i = 0; i < nts; i++){
        ucur[i] = ucur[i+nts] = 1.00/cosh(t[i]/2.0);
        help[i] = _real(ucur[i]);
        nvec1[i] = ucur[i];
    }
    for(int i = 0; i < num_pulses; i++){
        fft(ffor, nvec1 + i*nts, nvec1 +1*nts, nts);
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
            vv[j]->parse("0.1");
#ifdef gen_t_dat
            vv[j]->set(0*2*3.1415*(rand()*1.0/RAND_MAX));
#else
            vv[j]->set(0);
#endif
            invals[vv[j]->name()]=vv[j];
            cont->addvar(vv[j]);
        }

        jones_matrix* m = new jones_matrix(vv, get_unique_name(mat_base));
        invals[m->name()]=m;
        jones_matrices.push_back(m);
    }
}
double sign(double v){
    if(v < 0){
        return -1;
    }
    return 1;
}
/*!
 * FFT solutions since this is a spectral system
 */
void jones_optical::pre_fft_operations(){
    if(round==0){
        for(int i = 0; i < nts; i++){
            ucur[i] = ucur[i+nts] = 1.00/cosh(t[i]/2.0);
            help[i] = _real(ucur[i]);
        }
    }
    if(round == 1){
        for(size_t j = 0; j < num_pulses; j++){
            fft(ffor, ucur+j*nts, ucur+j*nts, nts);
        }
        for(size_t i = 0; i < dimension; i++){
            nvec1[i] = ucur[i];
        }
        for(size_t j = 0; j < num_pulses; j++){
            ifft(fback, ucur+j*nts, ucur+j*nts, nts);
        }
    }
/*    if(round==num_min){
        for(size_t j = 0; j < num_pulses; j++){
            fft(ffor, ucur+j*nts, ucur+j*nts, nts);
        }

        for(size_t i = 0; i < nts; i++){
            nvec2[i] = _sqabs(ucur[i]);        
            for(size_t j = 0; j < num_pulses; j++){
                nvec2[i] += _sqabs(ucur[i+j*nts]);
            }
            nvec2[i] = sqrt(nvec2[i]);
        }

        for(size_t i = 0; i < nts; i++){
            fprintf(func_dat, "%lf ", nvec2[i]);
        }
        fprintf(func_dat, "\n");
        for(size_t j = 0; j < num_pulses; j++){
            ifft(fback, ucur+j*nts, ucur+j*nts, nts);
        }
    }*/
}
void jones_optical::post_ifft_operations(){
    //apply the jones matrices, and integration between them

    if(!jones_matrices.empty()){
        Eigen::Map<Eigen::Matrix<comp, 2, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Aligned> dmap(ucur, 2, nts);
        dmap = jones_matrices[0]->mvals*dmap;
        for(int i = 1; i < jones_matrices.size(); i++){
            for(size_t j = 0; j < num_pulses; j++){
                fft(ffor, ucur+j*nts, ucur+j*nts, nts);
            }
            inter->integrate(ucur, tcur, tcur+jones_int_dist);
            for(size_t j = 0; j < num_pulses; j++){
                ifft(fback, ucur+j*nts, ucur+j*nts, nts);
            }
            tcur+=jones_int_dist;
            dmap = jones_matrices[i]->mvals*dmap;
        }
    }
}
double jones_optical::get_change(){
    double norm, change_norm;
    norm = change_norm = 0;
    for(size_t j = 0; j < nts; j++){
        double fsq = _sqabs(ulast[j]);
        for(size_t i = 1; i < num_pulses; i++){
            fsq += _sqabs(ulast[j+i*nts]);
        }
        norm += fsq; 
        double cfsq = _sqabs(ucur[j]) ;
        for(size_t i = 1; i < num_pulses; i++){
            cfsq += _sqabs(ucur[j+i*nts]);
        }
        cfsq = sqrt(cfsq)-sqrt(fsq);
        change_norm += cfsq*cfsq;
    }
    if(norm < 1e-4){
        return 0;
    }
    double ss = sqrt(change_norm/norm);
    if(round < 45 && ss < 1e-3){
        std::cout << "stable solution reached, norm not zero" << std::endl;
    }
    return ss;
}
double jones_optical::score(){
    double score = obj->score(ucur);
    data_store dat;
    dat.avals.resize(4*jones_matrices.size());
    for(size_t i = 0; i < jones_matrices.size(); i++){
        dat.avals[i*4]=jones_matrices[i]->a1;
        dat.avals[i*4+1]=jones_matrices[i]->a2;
        dat.avals[i*4+2]=jones_matrices[i]->a3;
        dat.avals[i*4+3]=jones_matrices[i]->ap;
    }
    dat.score = score;
    out_dat.push_back(dat);
    if(score > best_score){
        best_score = score;

        ba1=jones_matrices[0]->a1;
        ba2=jones_matrices[0]->a2;
        ba3=jones_matrices[0]->a3;
        bap=jones_matrices[0]->ap;
        for(size_t i = 0; i < nts; i++){
            phold[i] = _sqabs(ucur[i]);
            for(size_t j = 1; j < num_pulses; j++){
                phold[i] += _sqabs(ucur[i+j*nts]);
            }
            phold[i] = sqrt(phold[i]);
        }       
    }
    if(round < max_iterations - 1 && score > .05){
        fprintf(func_score, "%lf 1\n", score);
    }
    else{
        fprintf(func_score, "%lf 0\n", score);
    }
    return score;
}
jones_optical::~jones_optical(){
    FILE* pmax;
    pmax = fopen(out_f_name.c_str(), "w");
    std::list<data_store>::iterator it = out_dat.begin();
    it++;//discard this first since it has been replicated
    //This also provides a more chaotic base for the later simulations and increases accuracy
    for(; it != out_dat.end(); it++){
        for(size_t i = 0; i < it->avals.size(); i++){

            fprintf(pmax, "%lf ", it->avals[i]);
        }
        fprintf(pmax, "%lf\n", it->score);
    }
    fclose(pmax);
    printf("%f, %f, %f, %f\n", ba1, ba2, ba3, bap);
    pmax = fopen("pmax.out", "w");
    for(size_t i = 0; i < nts; i++){
        fprintf(pmax, "%d, %lf\n", (int)i, phold[i]);
    }
    pmax = fopen("diff.out", "w");
    for(size_t i = 0; i < dimension*2; i++){
        fprintf(pmax, "%d, %lf\n", (int)i, ((double*)nvec2)[i]);
    }

    fclose(pmax);
    al_free(help);
    al_free(t);
    al_free(kurtosis_help);
    al_free(phold);
    al_free(nvec1);
    al_free(nvec2);
    fclose(func_dat);
    fclose(func_score);
}

std::string jones_optical::type() const{
    return "jones_optical";
}
