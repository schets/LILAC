#ifndef VARIABLE_H
#define VARIABLE_H
#include "item.h"
#include "native_item.hpp"
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
#endif
