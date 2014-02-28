#include "hasher.h"
extern "C"{
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
}
#include <ctime>
#include <iostream>
hasher::hasher(size_t num_hash, size_t num_rand, std::vector<float> lens):hashmat(lens.size(), num_hash), 
        rand_scores(num_rand, num_hash){
    gsl_rng* rng;
    size_t dim = lens.size();
    rng = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(rng, time(0));
    Matrix<float, Dynamic, Dynamic> rand_vecs(num_rand, dim);
    //initialize the random position vectors
    for(size_t i = 0; i < num_rand; i++){
        for(size_t j = 0; j < dim; j++){
            rand_vecs(i, j) = gsl_rng_uniform(rng) * lens[j];
        }
    }
    //generate the random hash matrix, gaussian distributed
    //will use dummy variables for now, but will actually calculate them later in the future 
    double std_dev=1.0;
    for(size_t j = 0; j < num_hash; j++){
        for(size_t i = 0; i < dim; i++){
            hashmat(i, j)=fabs(gsl_ran_gaussian(rng, std_dev));
        }
    }
    //hash the random vectors with each hash
    //this could be done by actually dong the dot products,
    //but a matrix multiply will probably be quicker and is easier to code
    rand_scores.noalias()=rand_vecs*hashmat;
    gsl_rng_free(rng);
        }
hasher::~hasher(){
}

void hasher::hash(const Matrix<float, Dynamic, Dynamic, RowMajor>& invecs, Matrix<float, Dynamic, Dynamic, ColMajor>& hashes){
    hashes.noalias()=invecs*hashmat;
};
int main()
{
    std::vector<float> lens(8, 2*3.14159);
    hasher hh(2000, 100000, lens);
}
