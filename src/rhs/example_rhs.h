//! This files is the example header
/*! \file This is an example file for how to add a new function that can be integrated, for someone new to C++
* As of now, this represents the current state of the engine.
* Since the engine is still being developed, this example will change with it.
* For example, I already have some semi-significant changes planned to facilitate
* an effective method for referencing variables throughout the input file
* It still has good input on how to make an rhs-type class
*/
#include "rhs_type.hpp"//This file contains the template for the base class type, along with most basic headers
//The way this works is through polymorphism, which basically means that the engine sees a class of rhs, but is really accessing a class of type example_rhs


//!This class is a basic example of how to write an rhs class
/*!This class is an example of a function rhs class.
 * It implements all of the functions needed, and can even be called from
 * the input file. Make sure to read the source file, as it contains a large amount of documentations that is not present in the documentation
 * Notice that it inherits from rhs_type<comp>, since it is a complex value rhs.
 * An rhs that dealt with real variables would inherit from rhs_type<double>.
 *
 * \sa example_rhs.cpp, example_rhs.h, rhs, and item 
 */
class example_rhs:public rhs_type<comp>{
    //up here declare variables that are used internally by the function
    //These may be akin to various parameters, tunable or not
    
    //double represents the 64 bit floating point variables
    double val1, val2;
    
    //std::string represents the string type values
    std::string random_info;

    //comp values are the complex variable types
    //comp_funcs.h contains some optimized functions for manipulating complex variables
    comp something_useless;

    //pointers store a location in memory, and can act as arrays.
    //They offer high performance compared to other options,but also require
    //rather vigilant care since they directly manipulate memory
    //They are useful for storing temporary calculations
    comp* value_holder;

    public: //anything that isn't declared under public is only accessible through
    //functions of the class, but public members can be accessed outside of the class
    //
    //Functions of a c++ class are declared in a header file, but actually defined in a source file
    //This allows one to separate the implementation of the class from telling the engine
    //what the class is, ane makes everything work better in the long run
    //
    //PLEASE FOLLOW THIS PATTERN WITH USER DEFINIED CLASSES
    //
    //these functions delcared in the next block are utility functions
    //used by the engine to sort different parts of the simulation and
    //make everything work nicely together. In each class they must be declared exactly
    //as is here. The detailed documentation, and comments in the .cpp file,
    //have information on what these functions do

    virtual std::string type() const;
    virtual std::vector<std::string> dependencies() const;
    virtual void postprocess(input& indat);


    //This function updates data inside the class whenever a variable it references changes
    //use it to re-do any calculations that are done to initialize the class
    virtual void update();

    //This function calculates the derivative given the current function value
    //Note that instead of pointers, this class takes in an object of type
    //ptr_passer. This holds a pointer and type information to ensure that
    //the rest of the engine, which is quite generic and type-independent,
    //won't pass the wrong type of pointer to this function. Although the 
    //rest of the type-checking is probably strict enough to allow the use of
    //void*, this prevents a user from accidentally passing a bad pointer anyways.
    virtual int dxdt(ptr_passer x, ptr_passer dx, double t);
    
    //This is a destructor, which frees all of your memory
    //if you allocate any memory, you must free it
    virtual ~example_rhs();
};

