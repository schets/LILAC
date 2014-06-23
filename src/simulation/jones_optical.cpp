/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#ifdef CLANG
#pragma clang diagnostic ignored "-Wdeprecated-register"
#endif
#include "writer/writer.h"
#include "eigen3/Eigen/Eigen"
#include "jones_optical.h"
#include "utils/comp_funcs.hpp"
#include "utils/noise.hpp"
#include "types/type_register.hpp"
#include "engine/engineimp.h"

template class type_register<jones_optical>;
/*!
 * class to allow automatic updating of the matrices involved
 * It's hacky but there isn't a generic operator-creator made
 * for creating elements in the post processor of items yet
 */
class jones_matrix:public _double{

    public:
        double a1, a2, a3, ap;
        int num_seg;
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
            std::shared_ptr<writer> dat(new writer(true));
            dat->add_data(data::create("Segment", num_seg), writer::PARAMETER);
            double avals[4] = {a1, a2, a3, ap};
            dat->add_data(data::create("Angles", avals, 4), writer::PARAMETER);
            holder->add_writer(dat);
            j1=rmat(a1)*wq*rmat(-1*a1);
            j2=rmat(a2)*wq*rmat(-1*a2);
            j3=rmat(a3)*wh*rmat(-1*a3);
            jp=rmat(ap)*wp*rmat(-1*ap);
            mvals = j1*jp*j2*j3;
        }
        jones_matrix(std::string n, int seg, engineimp* hold){
            this->holder = hold;
            num_seg = seg;
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
            avars[0]->add_ref(a1, this);
            avars[1]->add_ref(a2, this);
            avars[2]->add_ref(a3, this);
            avars[3]->add_ref(ap, this);
            update();
        }
};
std::vector<std::string> jones_optical::dependencies() const{
    std::string deps[] = {"num_jones_segments", "jones_int_dist"};
    return make_append(deps, stable_spectral_pde_1d_tmpl<comp>::dependencies());
}
void jones_optical::postprocess(input& invals){
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
    invals.retrieve(num_segments, "num_jones_segments", this);
    if(num_segments < 0){
        err("Number of jones segments must be greater than or equal to zero",
                "jones_optical::postprocess", "system/jones_optical.cpp", FATAL_ERROR);
    }
    invals.retrieve(jones_int_dist, "jones_int_dist", this);
    if(jones_int_dist<0){
        err("The distance between jones segments, jones_int_dist,  must be greater than or equal to zero",
                "jones_optical::postprocess", "system/jones_optical.cpp", FATAL_ERROR);
    }

    memp.add(dimension, &nvec1);
    memp.add(nts, &help, &t, &kurtosis_help, &phold, &nvec2);
    double dt = 60.0/nts;
    gaussian_noise(ucur, dimension, 0, 0.2); 
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
            invals.insert_item(val);
            cont->addvar(val);
        }
        std::shared_ptr<jones_matrix> m = std::make_shared<jones_matrix>(get_unique_name(mat_base), i, holder);
        invals.insert_item(m);
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

}
/*!
 * This function applies the Jones matrices after each integration run.
 * In addition, exatra integration is performed between each set of matrices to simulate the
 * waveplates being a physical distance apart
 */
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
//!Finds the difference between each pulse. 
/*!
 * This functions finds how much the light pulse has changed on the round trip.
 * This comparison compares the magnitude of the laser, instead of including phase information as well
 */
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
    return sqrt(change_norm/norm);
}

//!Returns score that favors tight and high-energy single pulses \sa n_pulse_score
double jones_optical::score(){
    return obj->score(ucur);
}
jones_optical::~jones_optical(){
}

std::string jones_optical::type() const{
    return "jones_optical";
}
