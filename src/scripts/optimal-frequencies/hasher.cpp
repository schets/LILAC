#include "hasher.h"
#include <ctime>
#include <iostream>
#include <stdio.h>
hasher::hasher(size_t num_hash, size_t num_rand, vector<float> lens, size_t d_vals, float min_interest):hashmat(lens.size(), num_hash),  tor_hashes(d_vals+2), rand_scores(num_rand, num_hash),  rand_vecs(num_rand, lens.size()),d_test(d_vals, 0){
        mnorm = 0;
        for(size_t i = 0; i < lens.size(); i++){
            mnorm += lens[i]*lens[i];
        }
        mnorm = sqrtf(mnorm);
        //initialize eash individual hash table
        tor_hashes[0]=vector<vector<size_t> >(num_hash, vector<size_t>(ceil(mnorm/(min_interest/2.0)), 0));
        tor_hashes[1]=vector<vector<size_t> >(num_hash, vector<size_t>(ceil(mnorm/min_interest), 0));

        for(size_t i = 2; i < tor_hashes.size(); i++){
            min_interest *= 2;
            float gval = min_interest;
            size_t num_buckets = ceil(mnorm / gval); 
            tor_hashes[i] = vector<vector<size_t> >(num_hash, vector<size_t>(num_buckets, 0));
        }
        size_t dim = lens.size();
  //      rmf rand_vecs(num_rand, dim);
  
        //initialize the random position vectors
        for(size_t i = 0; i < num_rand; i++){
            for(size_t j = 0; j < dim; j++){
                rand_vecs(i, j) = (rand()*1.0/RAND_MAX)*lens[j];
            }
        }
        //generate the random hash matrix   
        for(size_t j = 0; j < num_hash; j++){
            float norm = 0;
            for(size_t i = 0; i < dim; i++){
                float rval = rand() * (1.0/RAND_MAX);
                hashmat(i, j)= rval;
                norm += rval*rval;
            }
            //normalize the vectors
            //don't believe they need to be 
            norm = 1.0/sqrtf(norm);
            for(size_t i = 0; i < dim; i++){
                hashmat(i, j) *= norm;
            }
        }
        rand_scores.noalias()=rand_vecs*hashmat;
}

hasher::~hasher(){
}
//returns the bin of the float, assuming it is between 0 and mnorm
inline size_t hasher::get_bin(float hval, size_t n_bins){
    size_t stemp = floor(n_bins*hval/mnorm);
    if(stemp >= n_bins){
        cout << hval << " " << n_bins << " " << mnorm << endl;
    }
    return stemp;
}
void hasher::hash(const rmf& invecs, cmf& hashes){
    hashes.noalias()=invecs*hashmat;
};
void hasher::add_vecs(const rmf& invecs){
    //hash the input vectors
    in_vecs.push_back(invecs);
    cmf temp( invecs*hashmat);
    add_scores(temp);
}
void hasher::add_scores(const cmf& hvals){
    for(size_t g = 0; g < tor_hashes.size(); g++){
        vector<vector<size_t> >& hvec = tor_hashes[g];
        for(size_t j = 0; j < (size_t)hvals.cols(); j++){
            for(size_t i = 0; i < (size_t)hvals.rows(); i++){
                //    increment corresponding hash table bin
                hvec[j][get_bin(hvals(i, j), hvec[j].size())]++;
            }
        }
    }
}
vector<float> hasher::get_cur_scores(){
    const rmf& hvals = rand_scores;
    vector<float> scores(tor_hashes.size()-2, 0);
    for(size_t g = 2; g < tor_hashes.size(); g++){
        vector<vector<size_t> >& hvec = tor_hashes[g];
        //preceding has table gives probability that distance is more than
        //current measure
        vector<vector<size_t> >& hvecm2 = tor_hashes[g-2];
        //gval hvec = dval[i]*2
        //gval hvecm1 = dval[i]
        //gval hvecm2 = dval[i]/2
        //if(!in hvecm1), pr bad is for 2*gval = dval*2
        //if(!in hvecm2), pr bad is for 2*gval = dval
        //if(in hvec) pr good is for gval/2 = dval
        //therefore need to be minus two hash funcs
        for(size_t i = 0; i < (size_t)hvals.rows(); i++){
            float pr_far;
            size_t num_near = 0;
            size_t num_far=0;
            pr_far = 0;
            size_t num_ave = 0;
            for(size_t j = 0; j < (size_t)hvals.cols(); j++){
                //    increment corresponding hash table bin
                //    gamma/2, 2*gamma, 1/2, 1/3 sensitive
                //    for now we treat not being near as zero probability of begin near
                //    for now, hold two counts: number of vectors for which
                //    the hash function claims the rvec is near
                //    and number of times the rvec claims it is not near a vector
                //
                //    if (n_near*((1/2)/(1/3) < n_far)) then
                //    say it is near a vector. Else, it is not
                size_t num_near_tmp = hvec[j][get_bin(hvals(i, j), hvec[j].size())];
                long num_far_tmp = (long)hvecm2[j][get_bin(hvals(i, j), hvec[j].size())];
                num_near += num_near_tmp;
                //if not zero, make zero. otherwise make 1
                num_far_tmp = (num_far_tmp == 0);
                num_far += num_far_tmp;
                pr_far += (pow(0.5, num_near_tmp) + num_far_tmp);
                num_ave += (1+num_far_tmp);
            }
            //cout << num_near*1.0/hvals.cols() << endl;
            pr_far/= num_ave;
        //    if(pr_far < 0.2){
       if(num_far < 1){ 
                scores[g-2]++;
            }
        }
        scores[g-2]/=rand_scores.rows();
    }
    size_t num_bel_min = 0;

    list<rmf>::iterator beg;
    for(size_t r = 0; r < (size_t)rand_vecs.rows(); r++){
        float mindis=mnorm;
        list<rmf>::iterator lcur;
        for(lcur=in_vecs.begin(); lcur != in_vecs.end(); lcur++){
            for(size_t i = 0; i < (size_t)lcur->rows(); i++){
                float cdist = (rand_vecs.row(r) - lcur->row(i)).norm();
                if(cdist < mindis){
                    mindis = cdist;
                }
            }
        }
        if(mindis < 0.25){
            num_bel_min++;
        }
    }
   // float act_bel_min = (1.0*num_bel_min)/(1.0*rand_vecs.rows());
    cout << scores[0]*rand_vecs.rows() << ", " << num_bel_min << endl;
    return scores;
}

int main()
{
    srand(time(0));
    size_t dim, num_vec, num_inc;
    float in_inc, mul_val;
    num_inc=1000;
    in_inc=.1;
    mul_val=3.14159*.7;
    dim=6;
    num_vec=500;
    std::vector<float> lens(dim, 2*3.14159);
    size_t d_vals = 5;
    float min_int = 0.25;
    hasher hh(15, 10000, lens, d_vals, min_int);
    rmf vvals(rmf::Zero(num_vec, dim));
    for(size_t g =0; g < num_inc; g++){
        //copy most recent row into beginning
        float cur_inc=in_inc;
        for(size_t j = 0; j < dim; j++){
            vvals(0, j) = fmod(vvals(num_vec-1, j)+cur_inc, lens[j]);
            cur_inc *= mul_val;
        }
        //create the matrix of search vectors
        for(size_t i = 1; i < num_vec; i++){
            cur_inc = in_inc;
            for(size_t j = 0; j < dim; j++){
                vvals(i, j) = fmod(vvals(i-1, j)+cur_inc, lens[j]);
                cur_inc *= mul_val;
            }
        }
        hh.add_vecs(vvals);
        cout << "getting_scores" << endl;
        vector<float> scores = hh.get_cur_scores();
        cout << scores[0] << endl;
    }
    hh.get_cur_scores();
}

