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
    std::list<std::shared_ptr<const data>> dvals;
    std::map<int, bool> will_write;
    friend class engineimp;
    virtual void write(std::ostream& ofile) const;
    public:
    typedef enum{
        FINAL_SCORE,
        FINAL_FUNC,
        INTERMEDIATE_FUNC,
        INTERMEDIATE_SCORE,
        INDEX,
        OTHER,
        BEGIN=FINAL_SCORE,
        END=OTHER} w_type;

    virtual void add_data(std::shared_ptr<const data> inval, const w_type dat_t);
    virtual void add_data(std::shared_ptr<const data> inval, const std::string dat_t);

    void set_rule(w_type inval, bool rule);
    void set_rule(std::string inval, bool rule);

    virtual ~writer();
    writer(bool def = false);
    writer(const std::map<int, bool>& writevals, bool def = false);
    //won't implement copy for now since brings up question of multiple data
    //writer(const writer& win);

    static w_type from_name(const std::string inval);
    //same as above except throws an error if bad name
    static w_type from_name_strict(const std::string inval);
    static bool is_w_type(int inval);
};
#endif
