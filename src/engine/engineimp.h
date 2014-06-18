#ifndef ENGINEIMP_H
#define ENGINEIMP_H
#include <list>
#include <iostream>
#include <fstream>
#include "item_wrapper.h"
class engine;
class writer;
item_wrapper eval_lisp(std::string in_cmd, std::string base_name,
        std::map<std::string, input>& inv, engineimp* en);
/*!
 *
 * This class is the actual engime implementation. It is kept separate from
 * the engine class to allow editing without having to recompile large
 * portions of the program. It also makes interfacing safer as one has limited
 * access to the internals of the engine.
 *
 * In the future, there may be different types of engine (MPI-based engines, 
 * pure learning engines, etc) and this will make it much easier to implement and work on those
 * w/o exposing the logic to the rest of the program. I do need to move some of the engineimp functions
 * to the engine class, however. But for now, I need to get the c-elegans code running and some
 * proper tutorials written
 *
 */
class engineimp{
    void read(std::istream& fstr);
    void _read(std::istream& fstr);
    void sort_pp();
    void execute_command(std::string inval);
    void update();
    std::string curdir;
    std::ofstream ofile;
    std::map<size_t, std::list<std::shared_ptr<writer>>> writers;
    std::map<std::string, item_wrapper > values;
    std::map<std::string, input> inputs;
    std::set<std::shared_ptr<item>> update_vars;
    void write_data(std::shared_ptr<writer> inval, std::ostream& fwrite);
    void write_individual_dat(const std::list<std::shared_ptr<writer>>& dats, size_t ind, std::ostream& ostr);
    public:
    void write_dat();
    void add_writer(const std::shared_ptr<writer>& wval);
    void needs_updating(std::string name);
    void needs_updating(std::shared_ptr<item> p);
    size_t index;
    //! Returns the current count for updating, aka removing items
    void remove_item(std::string name);
    //!Runs the engine
    void run();
    //!Queries whether an item exists or not
    bool item_exists(std::shared_ptr<item> val) const;
    bool item_exists(const std::string& val) const;
    //!Constructor, reads from an input file
    engineimp(const std::string& fname, const std::string& outname, const std::string& index);
    //!Deletes data
    ~engineimp();
    friend class engine;
    friend class item;
    friend class variable;
    friend item_wrapper eval_lisp(std::string in_cmd, std::string base_name,
            std::map<std::string, input>& inv, engineimp* en);
};

#endif
