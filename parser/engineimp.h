#ifndef ENGINEIMP_H
#define ENGINEIMP_H
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include "item.h"
class engine;
class writer;
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
    void read(std::ifstream& fstr);
    void _read(std::ifstream& fstr);
    void sort_pp();
    void execute_command(std::string inval);
    void update();
    std::string curdir;
    std::map<std::string, std::shared_ptr<item>> values;
    std::set<std::shared_ptr<item>> update_vars;
    std::list<std::shared_ptr<const writer>> dats;
    void write_data(std::shared_ptr<const writer> inval, std::ofstream& fwrite);
    public:
    void add_writer(std::shared_ptr<const writer> wval);
    void write_dat();
    void needs_updating(std::string name);
    void needs_updating(std::shared_ptr<item> p);
    //! Returns the current count for updating, aka removing items
    void remove_item(std::string name);
    //!Runs the engine
    void run();
    //!Queries whether an item exists or not
    bool item_exists(std::shared_ptr<item> val) const;
    bool item_exists(const std::string& val) const;
    //!Constructor, reads from an input file
    engineimp(const std::string fname, const std::string outname, const std::string index);
    //!Deletes data
    ~engineimp();
    friend class engine;
    friend class item;
};

#endif
