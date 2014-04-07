#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <iostream>
#include <memory>
class engineimp;
class item{
    protected:
    std::string _name;
    std::string _write_name;
    char has_write_name;
    public:

    engineimp* holder;
    static std::shared_ptr<item> create(std::string name, engineimp* rval);
    virtual void print() const;
    virtual void parse(std::string inval) = 0;
    virtual void retrieve(void* inval, item* caller);
    virtual void postprocess(std::map<std::string, std::shared_ptr<item>>& indat){};
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const = 0;
    virtual void update();
    item();
    virtual ~item();
    void setname(const std::string n);
    void set_write_name(std::string wname);
    const std::string& name()const;
    const std::string& write_name() const;
    friend class variable;
};

//class that describes items with a dimension
class item_dim:public item{
    public:
        size_t dimension;
        virtual void postprocess(std::map<std::string, std::shared_ptr<item>>& dat);
        virtual std::vector<std::string> dependencies() const = 0;
};
//!Stores a real-valued input
class real8:public item{
    double value;
    public:
    virtual void print() const;
    virtual std::vector<std::string> dependencies() const;
    virtual  std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval, item* caller);
    friend class variable;
};
//!stores a string valued input
class string:public item{
    std::string value;
    public:
    virtual void print() const;
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval, item* caller);
};

//!Stores an integer valued input
class integer:public item{
    int value;
    public:
    virtual void print() const;
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval, item* caller);
    friend class engineimp;
};
//!stores a real number that is varied during the simulation
/*!
 * This class stores a real number that is allowed to vary during the simulation
 * It stores all of the values that are linked to it and updates them upon the change
 * A method of setting a variable that does not involve linking the memory is also implemented
 * This function does not re-implement type, as for the purposes of dependencies it is a real
 * If one must check for being an instance of variable, the check dynamic_cast operator must be used
 *
 * currently, all values are set to ge between 0 and 2*pi. Again, this is just to satisfy the CNLS
 * situation and have a function search. The parser uses the real parser and sets the initial value to zero
 * As the engine framework is further built, the incrementing/decrementing interface
 * will be generalized to be useful for general pruposes, not simply plain toroidal searches
 * \sa real
 */
class variable:public real8{
    size_t update_count;
    //double* are not use by std since they are mostly to stack allocated mem
    std::map<item*, std::set<double*> > modifiers;
    std::map<std::weak_ptr<item>, std::set<double*>, std::owner_less<std::weak_ptr<item> > > safe_mods;
    public:
    double low_bound, high_bound, inc_size;
    virtual void print() const;
    virtual void retrieve(void* inval, item* caller);
    virtual void copy(double* inval);
    virtual void parse(std::string inval);
    void inc();
    void inc(double i);
    void set(double v);
};


//classes for testing graphsort, mainly to ensure that it finds cyclical and unsatisfied dependencies
class ftest1 : public real8{
    public:
        virtual std::vector<std::string> dependencies() const{
            std::string deps[] = {"var1"};
            return std::vector<std::string>(deps, deps+1);
        }
};

class ftest2 : public real8{
    public:
        virtual std::vector<std::string> dependencies() const{
            std::string deps[] = {"var2"};
            return std::vector<std::string>(deps, deps+1);
        }
};
class ftest3 : public real8{
    public:
        virtual std::vector<std::string> dependencies() const{
            std::string deps[] = {"var3", "var4"};
            return std::vector<std::string>(deps, deps+2);
        }
};
#endif

