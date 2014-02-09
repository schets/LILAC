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
 * In the future, there may be different types of engine (MPI-based engines, 
 * pure learning engines, etc) and his will make it much easier to implement those
 *
 */
class engineimp{
    std::map<std::string, item*> values;
    void read(std::ifstream& fstr);
    void _read(std::ifstream& fstr);
    void sort_pp();
    std::string curdir;
    void execute_command(std::string inval);
    std::set<item*> update_vars;
    void update();
    public:
    void needs_updating(std::string name);
    void needs_updating(item* p);
    //! Returns the current count for updating, aka removing items
    void remove_item(std::string name);
    //!Runs the engine
    void run();
    //!Queries whether an item exists or not
    char item_exists(item* val) const;
    //!Constructor, reads from an input file
    engineimp(const std::string fname);
    //!Deletes data
    ~engineimp();
    friend class engine;
    friend class item;
};

#endif
