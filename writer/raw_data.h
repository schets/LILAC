//!Structure definitions for holding raw data
#ifndef RAW_DATA_H
#define RAW_DATA_H
#include <list>
struct raw_data{
    comp* data_vals;
    double score;
    double t;
    std::list<variable*> params;
};
#endif
