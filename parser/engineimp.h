#ifndef ENGINEIMP_H
#define ENGINEIMP_H
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "item.h"
class engine;
/*!
 *
 * This class is the actual engime implementation. It is kept separate from
 * the engine class to allow editing without having to recompile large
 * portions of the program. It also makes interfacing safer as one has limited
 * access to the internals of the engine.
 *
 */
class engineimp{
    std::map<std::string, item*> values;
    void read(std::ifstream& fstr);
    //!This tracks what update the engine is on, so that variables can skip querying sometimes
    size_t update_count;
    public:
    //! Returns the current count for updating, aka removing items
    inline size_t get_update_count(){
        return update_count;
    }
    void run();
    char item_exists(std::string name) const;
    engineimp(const std::string fname);
    ~engineimp();
    friend class engine;
    friend class item;
};

#endif
