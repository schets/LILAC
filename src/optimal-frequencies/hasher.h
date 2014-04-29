#ifndef HASHER_H
#define HASHER_H
#include "../eigen3/Eigen/Eigen"
#include <vector>
#include <list>
#include <map>
//cause we really won't have namespace clashing here
using namespace Eigen;
using namespace std;
//more effecient to use others, but this works as well

typedef Matrix<float, Dynamic, Dynamic, ColMajor> cmf;
typedef Matrix<float, Dynamic, Dynamic, RowMajor> rmf;
typedef Matrix<float, Dynamic, Dynamic> mf;
class hasher{
    cmf hashmat;
    //outside vector is for each gamma val, middle is for each hash, and inner holds the bins 
    vector<vector<vector<size_t> > > tor_hashes;
    cmf rand_scores;
    list<rmf> in_vecs;
    rmf rand_vecs;
    float mnorm;
    inline size_t get_bin(float hval, size_t n_bins);
    void add_scores(const cmf& inscores);
    vector<float> d_test;
    public:
    vector<float> get_cur_scores();
    void add_vecs(const rmf& invecs);
    void hash(const rmf& invecs, cmf& hashes);
    hasher(size_t num_hash, size_t num_rand, vector<float> dims, size_t d_vals, float min_interest);
    ~hasher();
};
#endif
