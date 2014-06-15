#include "c_elegans.h"
#include "engine/input.h"
#include "utils/comp_funcs.hpp"
#include "utils/item_heads.hpp"
#include "types/type_register.hpp"
#include <vector>
#include <fstream>
#include "gaba_list.hpp"
#include <eigen3/Eigen/Core>
#include "writer/writer.h"
#include "engine/engineimp.h"
#include "controller/controller.h"
bool next_comb(std::vector<size_t>& inval, size_t max_num){
    class comb_helper{
        public:
            typedef std::vector<size_t>::iterator iter_t;
            typedef std::vector<size_t>::const_iterator c_iter_t;
            static bool do_next(iter_t cur, c_iter_t end, size_t max_num){
                if(cur == end){
                    //test to ensure valid ptrs
                    //returns zero, since no ablations usually means something else is important
                    return false;
                }
                iter_t temp = cur;
                temp++;
                //increment current element;
                (*cur)++;
                if((*cur) >= max_num){
                    (*cur) = 0;
                }
                //if is at end
                if(temp == end){
                    return (*cur) == 0;
                }
                //if the current value is now greater than
                //or equal to the next, increment
                //while loop allows bad inputs to reach valid combination states
                //v[0] < v[1] < v[2] etc;
                //also yes this has n^2 runtime. fortunately n is small, like around 4-5 at most
                //and is called fairly rarely AND is simple
                while(*cur >= *temp){
                    *cur = 0;
                    if(do_next(temp, end, max_num)){
                        return true;
                    }
                }
                return false;
            }
    };
    return comb_helper::do_next(inval.begin(), inval.end(), max_num);
}
constexpr size_t c_elegans::num_neur;
constexpr size_t c_elegans::dim_v;
constexpr size_t c_elegans::dim_s;
template class type_register<c_elegans>;
int c_elegans::dxdt(ptr_passer x,  ptr_passer dx, double dt){
    double* restr v = x;
    double* restr dv = dx;
    double* restr ds = dv+dim_v;
    double* restr s = v+dim_v;
    //map eigen arrays over input pointers
    Map<Array<double, dim_v, 1>> vmap(v);
    Map<Array<double, dim_v, 1>> dvmap(dv);
    Map<Array<double, dim_v, 1>> smap(s);
    Map<Array<double, dim_v, 1>> dsmap(ds);
    //calculations from matlab file
    sig = 1.0 / (1.0 + (-1*beta*(vmap-vmean)).exp());
    dsmap = ar*(sig * (1.0-smap)) - ad*smap;
   /* Iohm = (memG*(vmap - memV));
    Ielec = (gelec*laplacian*vmap.matrix()).array();
    Ichem = (gchem *
            (vmap*(AEchem_trans*smap.matrix()).array() 
             - (AEchem_trans*(smap*Echem).matrix()).array()));
    dvmap = (-1.0/tau)*(Iohm + Ielec + Ichem);*/
    dvmap = (-1.0/tau)*(
            (memG*(vmap - memV)) +
            (gchem * (vmap*(AEchem_trans*smap.matrix()).array() 
              - (AEchem_trans*(smap*Echem).matrix()).array())) +
            (gelec*laplacian*vmap.matrix()).array()
            );
    //current injection-for now is hard coded below, inj_nodes is empty here
    for(auto s : inj_nodes){
        dv[s] += (-1.0/tau)*cur_inj;
    }
    double amp = 2e4;
    dv[276]+= (1.0/tau)*amp;
    dv[278]+= (1.0/tau)*amp;

    for(auto val : abl_neur){
        dv[val] = 0;
        ds[val] = 0;
    }
    return 0;
}
int c_elegans::dwdt(ptr_passer x, ptr_passer _dw, double dt){
    double* dw = _dw;
    std::fill(dw, dw+num_neur*2, 1);
    //doesn't need to set abl_neur to zero, since that is done by mod_w
    return 0;
}
void c_elegans::mod_w(ptr_passer _w, double t){
    if(!abl_neur.empty()){
        double *w = _w;
        for(auto abl : abl_neur){
            w[abl]= w[abl+num_neur] = 0;
        }
    }
}
std::string c_elegans::type() const{
    return "c_elegans";
}
template<class sp_t>
void read_mat(std::string fname, sp_t& in_mat){
    //intel compiler requries this...
    std::ifstream in_f;
    in_f.open(fname.c_str());
    std::vector<Eigen::Triplet<double, int> > intr;
    while(!in_f.eof()){
        int row, col;
        double val=0;
        in_f >> row >> col >> val;
        intr.push_back(Triplet<double, int>(row-1, col-1, val));
    }
    in_mat.setFromTriplets(intr.begin(), intr.end());
}
/*!
 * This function does the processing for the c_elegans class.
 *
 * It initializes the various matrices and reads values from the input files
 */
void c_elegans::postprocess(input& in){
    rhs_type::postprocess(in);
    if(dimension != num_neur*2){
        err("Dimension must be 558, which is double the number of neurons",
                "", "", FATAL_ERROR);
    }
    in.retrieve(beta, "beta", this);
    in.retrieve(tau, "tau", this);
    in.retrieve(gelec, "gelec", this);
    in.retrieve(gchem, "gchem", this);
    in.retrieve(memV, "memV", this);
    in.retrieve(memG, "memG", this);
    in.retrieve(EchemEx, "EchemEx", this);
    in.retrieve(EchemInh, "EchemInh", this);
    in.retrieve(ar, "ar", this);
    in.retrieve(ad, "ad", this);
    std::string ag_fname, a_fname;
    in.retrieve(ag_fname, "ag_mat", this);
    in.retrieve(a_fname, "a_mat", this);
    sparse_type a_m(num_neur, num_neur);
    ag_full.resize(num_neur, num_neur);
    laplacian.resize(num_neur, num_neur);
    read_mat(ag_fname, ag_full);
    read_mat(a_fname, a_m);
    //create transposed sparse matrix AEchem
    AEchem_trans_full.resize(num_neur, num_neur);
    AEchem_trans_full = a_m.transpose();
    AEchem_trans.resize(num_neur, num_neur);
    //do any needed fake iterations, must make more general at some point
    size_t num_comb;
    int iterations;
    in.retrieve(num_comb, "num_comb", this);
    in.retrieve(iterations, "iterations", this);
    in.retrieve(cur_ind, "!start_ind", this);
    abl_neur.resize(num_comb);
    for(auto& val : abl_neur){
        val = 0;
    }
    if(abl_neur.size() != 1){
        next_comb(abl_neur, num_neur);
    }
    for(int i = 0; i < cur_ind; i++){
        for(int j = 0; j < iterations; j++){
            if(next_comb(abl_neur, num_neur)){
                char ind_str[20];//won't ever have a 20 digit index
                //handy buffer to overflow for those hacking this.
                sprintf(ind_str, "%d", (int)cur_ind);
                err(std::string("Combinations exhausted in index ") + ind_str,
                        "c_elegans::postprocess","rhs/c_elegans.cpp", FATAL_ERROR);
            }
        }
    }
    //set up dummy connection to toroidal controller for now
    controller* cont;
    in.retrieve(cont, "controller", this);
    auto val = std::make_shared<variable>();
    val->setname(get_unique_name(name()));
    val->holder = holder;
    val->parse("0.1");
    in.insert_item(val);
    cont->addvar(val);
    in.retrieve(dummy, val->name(), this);
    update();
}
void c_elegans::update(){
    static bool has_gone = (cur_ind == 0) && (abl_neur.size() != 1);
    if(has_gone){
        if(next_comb(abl_neur, num_neur)){
            holder->write_dat();
            char ind_str[20];//won't ever have a 20 digit index
            //handy buffer to overflow for those hacking this.
            sprintf(ind_str, "%d", cur_ind);
            err(std::string("Combinations exhausted in index ") + ind_str,
                    "c_elegans::update","rhs/c_elegans.cpp", FATAL_ERROR);
        }
    }
    //this skips the first update, since postprocessing has done that
    else{
        has_gone = true;
    }
    auto dat_inds =
        std::shared_ptr<writer>(new writer(true));
    dat_inds->add_data(data::create("Ablations", abl_neur.data(), abl_neur.size()), writer::OTHER);
    holder->add_writer(dat_inds);
    Matrix<double, Dynamic, Dynamic> ag_dense(num_neur, num_neur);
    //insert code to zero it out later

    auto ag_m = ag_full;
    AEchem_trans = AEchem_trans_full;
    auto fncval = [this](int i, int j, double val)->bool{
        for(int kk = 0; kk < (int)this->abl_neur.size(); kk++){
            if((int)this->abl_neur[kk] == i || (int)this->abl_neur[kk] == j){
                return false;
            }
        }
        return true;
    };
    for(auto val : abl_neur){
        std::cout << val << ", ";
    }
    std::cout << std::endl;
    AEchem_trans.prune(fncval);
    ag_m.prune(fncval);
    ag_dense = ag_m *  Matrix<double, num_neur, num_neur>::Identity();
    auto sumvals = ag_dense.colwise().sum();
    sparse_type temp(num_neur, num_neur);
    //generate the sparse diagonal matrix to build the lapacian
    std::vector<Triplet<double, int> > temp_tr;
    for(int i = 0; i < (int)num_neur; i++){
        temp_tr.push_back(Triplet<double, int>(i, i, sumvals[i]));
    }
    temp.setFromTriplets(temp_tr.begin(), temp_tr.end());
    laplacian = temp - ag_m;
    //initialize the Echem array
    for(size_t i = 0; i < num_neur; i++){
        if(GABAergic[i]){
            Echem[i] = EchemInh;
        }
        else{
            Echem[i] = EchemEx;
        }
    }
    //Initialize the sig array
    for(size_t i = 0; i < num_neur; i++){
        sig[i] = 0.5;
    }

    eqS = sig * (ar/(ar*sig + ad));
    //more initialization of temporary dense matrices
    Matrix<double, Dynamic, Dynamic> ldense(num_neur,num_neur);
    ldense = laplacian*Matrix<double, num_neur, num_neur>::Identity();
    Matrix<double, Dynamic, Dynamic> aedense(num_neur, num_neur);
    aedense= AEchem_trans*Matrix<double, num_neur, num_neur>::Identity();
    Matrix<double, Dynamic, Dynamic> C(num_neur, num_neur);
    //create the C matrix
    C= memG*Matrix<double, num_neur, num_neur>::Identity() + gelec*ldense;
    //initialize matrix to modify diagonal part of C
    Matrix<double, num_neur, 1> tmp =(gchem * aedense * eqS.matrix()); 
    for(size_t i = 0; i < num_neur; i++){
        C(i, i) += tmp(i);
    }
    Matrix<double, num_neur, 1> Ivals;
    Ivals.setZero();
    double amp=2e4;
    Ivals[276]=amp;
    Ivals[278]=amp;
    Matrix<double, num_neur, 1> b;
    //create B vector
    b= gchem*aedense*(eqS * Echem).matrix();
    for(size_t i = 0; i < num_neur; i++){
        b[i] += (memG*memV + Ivals[i]);
    }
    //calculate eqV
    eqV.matrix() = C.inverse()*b;
    vmean = eqV+(1.0/beta) * (1.0/sig - 1).log();
    for(auto val : abl_neur){
        eqV[val] = vmean [val] = eqS[val] = 0;
    };
}

std::vector<std::string> c_elegans::dependencies() const{
    std::string deps[] = {"beta", "memV", "memG", "gchem", "gelec", "num_comb",
        "tau", "EchemEx", "EchemInh", "ar", "ad", "ag_mat", "a_mat", "iterations",
        "controller"};
    return make_append(deps, rhs_type::dependencies());
}

void c_elegans::initial_condition(ptr_passer in, size_t len){
    if(len != dimension){
        rhs_type<double, rhs_sde>::initial_condition(in, len);
    }
    double* vals = in;
    for(size_t i = 0; i < num_neur; i++){
        //vals[i] = eqV[i];
        vals[i] = vmean[i];
        vals[i+dim_v] = eqS[i];
    }
}
