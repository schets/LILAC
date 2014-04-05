//! This file is the source file for the example header
/*! \file This is the .cpp file for the example rhs function, with docs for someone new to C++
 * It contains some basic test functions, as well as extra in-function
 * documentation explaining most steps and various details
 * */
#include "example_rhs.h"
#include "comp_funcs.h" //includes various functions for complex variables
//****IMPORTANT*****IMPORTANT*****IMPORTANT*****IMPORTANT*****IMPORTANT*
//To have code that compiles with the engine, you need to edit the makefile in
//the rhs directory. It is the file that controls when pieces of code are built.
//Alhough it is possible to simply recompile everything every time, it is incredibly ineffecient
//and wastes and incredible amount of time.
//
//An example can be found, for this file, in the makefile in rhs.
//You pretty much just need to copy and paste a version with your file names
//instead of the example file names. Also, please don't remove the example build commands


//!This is deprecated, just implement an empty version for now
void example_rhs::parse(std::string inval){
}


//!This function returns the variable type
/*!
 * This function returns a string that has the typename of your variable. It is easiest to
 * simply have this match the name of your class, and it should definitely match the name 
 * that is used to create the class
 * @return Name of the class type
 */
std::string example_rhs::type() const{
    return "example_rhs";
}

//!This function returns a list of the dependencies needed to initialize the class
/*!The engine ensures that these variables will themselves be initialized
* when they are used to initialize the class. An error occurs if the variables are not found
* in the input or there is a loop of dependencies, this is a depends on b, which depends on c,
* which in turn depends back on a.
*
* \note
* This class depends on val1, val2, and random_info
* @return List of dependencies
*/
std::vector<std::string> example_rhs::dependencies() const{
    int dep_num=3;//number of dependencies
    std::string deps[] = {"val1", "val2", "random_info"};
    //this array holds the dependencies

    //this moves the dependencies into a container
    std::vector<std::string> strvec = std::vector<std::string>(deps, deps+dep_num);
    
    return appendvec(strvec, rhs::dependencies());
    //return the dependencies for this class along with any dependencies the parent classes have
}

//!This function processes the input data for the class
/*!
 * This function process data that has been generated from an input file
 * All of the names returned in the dependencies file exist and have been processed
 * @param dat The data structure containing the variable names
 */
void example_rhs::postprocess(std::map<std::string, std::shared_ptr<item> >& dat){
    rhs::postprocess(dat);//always postprocess the parent class first
    //Any class that inherits from rhs, or item_dim, has access to a variable
    //called dimension, which represents the dimension of the problem at hand
    //dimension is initialized in the item_dep, which is why the parent postprocessing
    //must be called first
    

    //There are two types of errors, warning and fatal errors.
    //Warnings print something to the screen to warn the user,
    //but do not stop the program. A fatal error causes an immediate exit
    //These are caused by the function err. Details can be found in the documentation
    //
    //Lets throw a fatal error if the dimension is less than 10
    if(dimension > 10){
        err("dimension > 10, which is required for example_rhs", "example_rhs::postprocess",
                "rhs/example_rhs.cpp", FATAL_ERROR);
    }
    
    //Lets also throw an error if the dimension is equal to 5
    if(dimension==5){
        err("dimension=5, which gets a warning from example_rhs", "example_rhs::postprocess",
                "rhs/example_rhs.cpp", WARNING);
    }
    //The map contains item*, which point to an item of arbitrary type.
    //I plan to implement type checking of the dependencies.
    //But sometime later
    //
    //To retrieve a value, you can call the retrieve function of the variable at hand
    //you just pass the name of parameter that is being retrieved to the map, which returns
    //an item*. Then this pointer is called to retrieve the value, and is passed the address
    //of val1

    dat["val1"]->retrieve(&val1, this);
    dat["val2"]->retrieve(&val2, this);
    dat["random_info"]->retrieve(&random_info, this);

    //now, we are going to allocate some memory to something
    //This may be useful for storing temporary calculations during the RHS
    //You should allocate pointers here, and then deallocate them in the dstructor.
    //allocating/unallocating on each call to dxdt is a great way to increase computation time
    //by an order of magnitude for most cases of dxdt
    //
    //I use al_malloc here instead of malloc or new, since it provides
    //aligned memory and plays nicely with fftw and Eigen.
    value_holder = (comp*)al_malloc(dimension*sizeof(comp));
    for(size_t i = 0; i < dimension; i++){
        value_holder[i] = Id*(double)i*val1 + (dimension-i)*val2;
    }
}
//!Update function for example_rhs
/*!
 * This function updates the values held by this class in case one of them
 * is a variable that is being controlled. We don't re-allocate memory,
 * since integers like dimension cannot be controlled. However, we must re-evaluate
 * the array value_holder. The values val1, val2, etc are controlled themselves
 * by the controller
 * 
 */
void example_rhs::update(){
    for(size_t i = 0; i < dimension; i++){
        value_holder[i] = Id*(double)i*val1 + (dimension-i)*val2;
    }
}

//!Destructor for example_rhs
/*!
 * This is the destructor for example_rhs. Whenever the class is deleted by the engine,
 * this frees the memory used by the class
 * Use it to deallocate pointers and other reources (fftw plans, for example)
 */
example_rhs::~example_rhs(){
    al_free(value_holder);
}


//!This is the rhs function that calculates the derivative
/*!
 * This functions calculates a derivative given the current time and function value
 * it takes in a pointer to x, which is the function value, and stores that data in dx.
 *
 * @param x The current function value
 * @param dx The array i which the derivative is stored
 * @param t The current time
 */
int example_rhs::dxdt(comp* restr x, comp* restr dx, double t){
    //This doesn't really do anything useful
    //but showcases some of the compex variable functions
    //See detailed documentation for more
    //But is a nice example
    for(size_t i = 0; i < dimension; i++){
        dx[i] = (_real(x[i])*_imag(value_holder[i])) * (val2/val1);
    }
    return 0;
}
