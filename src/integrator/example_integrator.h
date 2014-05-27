#ifndef EXAMPLE_INTEGRATOR_H
#define EXAMPLE_INTEGRATOR_H
//example_rhs should be studied before this class
#include "engine/item.h"
#include "integrator.h"
class example_integrator : public integrator{
    //This holds a pointer to what will be the actual implementation
    //We do this since the actual implementation is a template, so that it can integrate generic
    //types without the performance problems of dynamic polymorphism
    //!Holds a collected pointer to the actual integrator being used
    std::shared_ptr<example_integrator> actual_int;
    public:
        //This is the definition of the integrate function
        //Nothing too interesting here, but \sa example_integrator_tmpl::integrate
        //!\sa integrator::integrate
        int integrate(ptr_passer u, double t0, double tf);

        //Returns the type of the current class

        //This is the name that one would write in the input file to initialize this class
        //!Returns the typename of the current class
        std::string type() const;

        //!Returns the variables that this class depends on
        /*!
         *  This class depends on 
         *      - double rval1: Some parameter
         *      - int rval2: Some other parameter, optional value of -1
         *      - unsigned unsigned_var: Something random
         *      - string something: Some third parameter
         */
        std::vector<std::string> dependencies() const;

        //This function processes the input data after being read from the file
        //This is only called once, at the beginning of the program
        //Updates from variables must be done in the update function
        //!Processes the input data
        void postprocess(input& inval);

        //This returns the variable type of the integrator
        //This is actually going to be done by the integrator
        //!Returns the type of the integrator \sa vartype
        const std::type_info& vtype() const;

};
#endif
