#ifndef ITEM_H
#define ITEM_H
#include <map>
#include <set>
#include <iostream>
#include "utils/defs.hpp"
class engineimp;
class retrieve_wrapper;
class item;

template<class T>
void retrieve(T& inval, item* sender, item* caller);
class input;
class native_item;
class item{
    protected:
    //_retrieve should never be called by itself, only through the retrieve function
    //It doesn't really matter whether retrieve is public/private/whatever,
    //since nothing besides safe_retrieve can construct __retrieve_checkers
    //Any class overloading this should think carefully about what it's purpose is,
    //since anything that doesn't hold a native value is likely better off as an item*
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    std::string _name;
    std::string _write_name;
    char has_write_name;
    public:
    item();
    virtual ~item();
    engineimp* holder;
    static std::shared_ptr<item> create(std::string name, engineimp* rval);
    virtual void print() const;

    virtual void postprocess(input& indat);
    virtual std::vector<std::string> dependencies() const;
    virtual void update();

    virtual std::string type() const = 0;
    void setname(const std::string n);
    void set_write_name(std::string wname);
    const std::string& name()const;
    const std::string& write_name() const;

    template<class T> friend void retrieve(T& inval, item* setter, item* caller);
    friend class input;
    friend class native_item;
};
class native_item:public item{
    protected:
        //move tom protected scope so native values can override it
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    public:
        virtual void parse(const std::string& inval) = 0;
        //native items can't have dependencies or perform postprocessing
        std::vector<std::string> dependencies() const final_def;
        void postprocess(input& inval) final_def;
};
//!Stores a real-valued input
class _double:public native_item{
    double value;
    public:
    virtual void print() const;
    virtual  std::string type() const;
    virtual void parse(const std::string& inval);
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    friend class variable;
};
//!Stores a reduced-precision real valued input
class _float:public native_item{
    size_t value;
    public:
    virtual void print() const;
    virtual std::string type() const;
    virtual void parse(const std::string& inval);
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    friend class engineimp;
};

//!stores a string valued input
class string:public native_item{
    std::string value;
    public:
    virtual void print() const;
    virtual std::string type() const;
    virtual void parse(const std::string& inval);
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
};

//!Stores an integer valued input
class integer:public native_item{
    int value;
    public:
    virtual void print() const;
    virtual std::string type() const;
    virtual void parse(const std::string& inval);
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    friend class engineimp;
};
//!Stores an unsigned integer as input
class _unsigned:public native_item{
    size_t value;
    public:
    virtual void print() const;
    virtual std::string type() const;
    virtual void parse(const std::string& inval);
    virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    friend class engineimp;
};

#ifdef ICC
//!helper function for variable class
bool operator < (const std::weak_ptr<item>& a, std::weak_ptr<item>& b);
#endif
//!stores a real number that is varied during the simulation
/*!
 * This class stores a real number that is allowed to vary during the simulation
 * It stores all of the values that are linked to it and updates them upon the change
 * A method of setting a variable that does not involve linking the memory is also implemented
 * This function does not re-implement type, as for the purposes of dependencies it is a real
 * If one must check for being an instance of variable, the check dynamic_cast operator must be used
 *
 * currently, all values are set to ge between 0 and 2*pi. Again, this is just to satisfy the CNLS
 * situation and have a function search. The engine uses the real parser and sets the initial value to zero
 * As the engine framework is further built, the incrementing/decrementing interface
 * will be generalized to be useful for general pruposes, not simply plain toroidal searches
 * \sa real
 */
class variable:public _double{
	//double* are not use by std since they are mostly to stack allocated mem
	std::map<item*, std::set<double*> > modifiers;
#ifdef ICC
	std::map<std::weak_ptr<item>, std::set<double*>> safe_mods;
#else
	std::map<std::weak_ptr<item>, std::set<double*>,
		std::owner_less<std::weak_ptr<item>>> safe_mods;
#endif
	public:
	double low_bound, high_bound, inc_size;
	virtual void print() const;
	virtual void _retrieve(retrieve_wrapper&& inval, item* caller);
    virtual void add_ref(double& in, std::shared_ptr<item> inval);
    virtual void add_ref(double& in, item* inval);
	virtual void copy(double* inval);
	virtual void parse(const std::string& inval);
	virtual std::string type() const;
	void inc();
	void inc(double i);
	void set(double v);
};


//classes for testing graphsort, mainly to ensure that it finds cyclical and unsatisfied dependencies
class ftest1 : public item{
	public:
		virtual std::vector<std::string> dependencies() const{
			std::string deps[] = {"var1"};
			return std::vector<std::string>(deps, deps+1);
		}
		virtual std::string type() const{
			return "ftest1";
		}
};

class ftest2 : public item{
	public:
		virtual std::vector<std::string> dependencies() const{
			std::string deps[] = {"var2"};
			return std::vector<std::string>(deps, deps+1);
		}
		virtual std::string type() const{
			return "ftest2";
		}
};
class ftest3 : public item{
	public:
		virtual std::vector<std::string> dependencies() const{
			std::string deps[] = {"var3", "var4"};
			return std::vector<std::string>(deps, deps+2);
		}
		virtual std::string type() const{
			return "ftest3";
		}
};
#endif

