#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <map>
#include <vector>
#include <list>
#include <iostream>
class engineimp;
class item{
    std::string _name;
    engineimp* holder;
    public:
    static item* create(std::string name, engineimp* rval);
    virtual void print() const;
    virtual void parse(std::string inval) = 0;
    virtual void retrieve(void* inval);
    virtual void postprocess(std::map<std::string, item*>& indat){};
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const = 0;
    virtual ~item(){};
    void setname(const std::string n);
    const std::string& name()const ;
};

//class that describes items with a dimension
class item_dim:public item{
    public:
        size_t dimension;
        virtual void postprocess(std::map<std::string, item*>& dat);
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
    virtual void retrieve(void* inval);
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
    virtual void retrieve(void* inval);
};

//!Stores an integer valued input
class integer:public item{
    int value;
    public:
    virtual void print() const;
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval);
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
    std::list<double*> modifiers;
    public:
    double low_bound, high_bound, inc_size;
    virtual void print() const;
    virtual void retrieve(void* inval);
    virtual void copy(double* inval);
    virtual void parse(std::string inval);
    void inc();
};

/*!
 * This class that a children have a pointer to a set of frequency values
 * Has no functions, and does not inherit from anything else
 */
class spectral{
    public:
        double* k;
};
#endif

