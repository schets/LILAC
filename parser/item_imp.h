#ifndef ITEM_IMP_H
#define ITEM_IMP_H
#include <list>
//!Stores a real-valued input
class real:public item{
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
 * As the engine framework is further built, the incrementing/decrementing interface
 * will be generalized to be useful for general pruposes, not simply plain toroidal searches
 * \sa real
 */
class variable:public real{
    std::list<double*> modifiers;
    public:
    double low_bound, high_bound, inc_size;
    virtual void print() const;
    virtual void retrieve(void* inval);
    virtual void copy(double* inval);
    void inc();
};
#endif
