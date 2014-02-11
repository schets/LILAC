#ifndef WRITER_H
#define WRITER_H
#include "raw_data.h"
/*
 * I have no idea what the data io api or interface is going to look like,
 * for now it is up to the system class to deal with that
 * I think that once I have a better idea of how data will be used by this program, and
 * dicuss the issue with group members, this part can be developed
 */
//! This class forms the api for data storage/output
class writer:public item{
    public:
        void add_data(const raw_data& dat) = 0;
        void write() = 0;
};
#endif
