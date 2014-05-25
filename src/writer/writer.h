#pragma once
#ifndef WRITER_H
#define WRITER_H
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include "utils/defs.hpp"
#include "data.hpp"
class writer{
    //!Holds each data point
    std::list<std::shared_ptr<const data>> dvals;
    //!Maps enum parameters to a bool deciding whether that label will write or not
    std::map<int, bool> will_write;
    friend class engineimp;
    //!Writes the current data to a file
    virtual void write(std::ostream& ofile) const;
    static bool is_w_type(int inval);
    public:
    //!Labels for various input types
    typedef enum{
        FINAL_SCORE,
        FINAL_FUNC,
        INTERMEDIATE_FUNC,
        INTERMEDIATE_SCORE,
        INDEX,
        PARAMETER,
        OTHER,
        BEGIN=FINAL_SCORE,
        END=OTHER} w_type;

    virtual void add_data(std::shared_ptr<const data> inval, const w_type dat_t);

    void set_rule(w_type inval, bool rule);

    virtual ~writer();
    writer(bool def = true);
    writer(const std::map<int, bool>& writevals, bool def = true);
};
#endif
