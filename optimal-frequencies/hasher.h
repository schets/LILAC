#ifndef HASHER_H
#define HASHER_H
#include "../eigen3/Eigen/Eigen"
#include <vector>
//cause we really won't have namespace clashing here
using namespace Eigen;
class hasher{
    std::vector<float> r_hashes;
    Matrix<float, Dynamic, Dynamic, ColMajor> hashmat;
    public:
    Matrix<float, Dynamic, Dynamic, ColMajor> rand_scores;
    void hash(const Matrix<float, Dynamic, Dynamic, RowMajor>& invecs, Matrix<float, Dynamic, Dynamic, ColMajor>& hashes);
    hasher(size_t num_hash, size_t num_rand, std::vector<float> dims);
    ~hasher();
};
#endif
