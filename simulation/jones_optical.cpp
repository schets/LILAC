#include "writer/writer.h"
#include "jones_optical.h"
#include "comp_funcs.h"
#include "utils/noise.h"
#include <set>
#include <cstdlib>
#include <ctime>
#include <cmath>
double mom4(comp invals[], double* kvals, int len);
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
        jones_matrix(std::string n){
            setname(n);

        }
        void setup(std::vector<std::shared_ptr<variable> > avars){
            wq(1, 0) = wq (0, 1) = 0;
            wq(0, 0)=std::exp(-1.0*Id*3.14159*0.25);
            wq(1, 1)=std::exp(1.0*Id*3.14159*0.25);
            wh(1, 0)=wh(0, 1)=0;
            wh(0, 0)=-1.0*Id;
            wh(1, 1)=1.0*Id;
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
void jones_optical::postprocess(std::map<std::string, std::shared_ptr<item> >& invals){
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

    memp.add(dimension, &nvec1);
    memp.add(nts, &help, &t, &kurtosis_help, &phold, &nvec2);
    double dt = 60.0/nts;
    gaussian_noise_double(ucur, dimension, 0.2); 
    for(size_t i = 0; i < nts; i++){
        t[i] = dt*(i-nts/2.0);
    }
    for(size_t i = 0; i < nts; i++){
        ucur[i] = ucur[i+nts] = 1.00/cosh(t[i]/2.0);
        help[i] = _real(ucur[i]);
        nvec1[i] = ucur[i];
    }
    for(size_t i = 0; i < num_pulses; i++){
        fft(nvec1 + i*nts, nvec1 +1*nts, nts);
    }
    //generate variables for the jones matrices
    //create jones matrices
    std::string name_base = "jones_system_vars";
    std::string mat_base = "jones_system_matrices";

    for(int i = 0; i < num_segments; i++){
        std::vector<std::shared_ptr<variable> > vv(4);
        for(auto& val : vv){
            val = std::make_shared<variable>();
            val->setname(get_unique_name(name_base));
            val->holder=holder;
            val->parse("0.1");
#ifdef gen_t_dat
            val->set(0*2*3.1415*(rand()*1.0/RAND_MAX));
#else
            val->set(0);
#endif
            invals[val->name()]= val;
            cont->addvar(val);
        }
        std::shared_ptr<jones_matrix> m = std::make_shared<jones_matrix>(get_unique_name(mat_base));
        invals[m->name()]= m;
        m->setup(vv);
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
        for(size_t i = 0; i < nts; i++){
            ucur[i] = ucur[i+nts] = 1.00/cosh(t[i]/2.0);
            help[i] = _real(ucur[i]);
        }
    }
    if(round == 1){
        for(size_t j = 0; j < num_pulses; j++){
            fft(ucur+j*nts, ucur+j*nts, nts);
        }
        for(size_t i = 0; i < dimension; i++){
            nvec1[i] = ucur[i];
        }
        for(size_t j = 0; j < num_pulses; j++){
            ifft(ucur+j*nts, ucur+j*nts, nts);
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
        //this is a more complicated structure but will save an fft/ifft combination
        dmap = jones_matrices[0]->mvals*dmap;
        for(size_t i = 1; i < jones_matrices.size(); i++){
            for(size_t j = 0; j < num_pulses; j++){
                fft(ucur+j*nts, ucur+j*nts, nts);
            }
            inter->integrate(ucur, tcur, tcur+jones_int_dist);
            for(size_t j = 0; j < num_pulses; j++){
                ifft(ucur+j*nts, ucur+j*nts, nts);
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
  /*  al_free(help);
    al_free(t);
    al_free(kurtosis_help);
    al_free(phold);
    al_free(nvec1);
    al_free(nvec2);*/
    fclose(func_dat);
    fclose(func_score);
}

std::string jones_optical::type() const{
    return "jones_optical";
}
