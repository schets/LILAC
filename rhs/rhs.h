#ifndef RHS_H
#define RHS_H
#include "defs.h"
#include "parser/item.h"
/*!
 * The RHS class is effectively the definition of the equations that are being solved.
 * From the perspective of the integrator, and the solver, this is a black box that
 * creates a function's derivative given the value. They just see an rhs* and call dxdt. All classed that inherit rhs
 * must provide a calling function that takes in the function values at a certain point
 * and an array that will contain the derivatives at that time, along with the current time
 * The return value of this function can be an error code, although I have not implemented
 * any error handling yet. The C++ try/catch functionality wreaks havoc on optimizations
 * so I don't think it would be a good idea to use it in any of the routines here
 *
 *
 * 
 * */
class rhs:public item{

    protected:
        /*!
         * Stores the dimension of the rhs 
         * */
        size_t dimension;
    public:
        /*!\brief
         * Returns the dimension of the right hand side
         */
        inline size_t dim() const{
            return dimension;
        }
        static rhs* create(std::string tname);
        virtual std::vector<std::string> dependencies() const;
        virtual void parse(std::string inval);
        virtual void retrieve(void* inval) const ;
        /*!\brief
         * Creates the base rhs class with a dimension=dimen
         */
        rhs(){};
        rhs(size_t dimen): dimension(dimen){}
        /*!\brief
         * Stores the derivative in dx given the time t and current x
         */
        virtual int dxdt(comp* restr x, comp* restr dx, double t) = 0;
        virtual ~rhs(){};
};

#endif
