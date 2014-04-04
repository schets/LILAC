#pragma once
#ifndef WRITER_H
#define WRITER_H
#include <string>
#include <sstream>
#include <list>
#include "utils/defs.hpp"
#include "data.h"
class writer{
    std::list<data*> dvals;
    public:
    virtual void add_data(data* inval);
    virtual ~writer();
};
#endif
