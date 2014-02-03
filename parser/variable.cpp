#include "defs.h"
#include "item.h"
/*!
 * This function prints the value of the current variable
 */
void variable::print() const{
    std::cout << "variable " << this->name() << "=" << value;
}

/*!
 * This function assigns the real value stored to the value pointed
 * at by inval. It also keeps track of the pointer and updates it whenever
 * the variable has changed
 */
void variable::retrieve(void* inval){
    double* d = (double*)inval;
    *d = value;
    modifiers.remove(d);
    modifiers.push_back(d);
}

/*!
 * This function increments the variable.
 * The list of pointers will be managed by the engine class,
 * but for now it is unsafe. Currently the goal is to get a
 * functioning automated toroidal search, and move on from there
 */
void variable::inc(){
    value += inc_size;
    //deal with periodicity here
    if(value > high_bound){
        value = low_bound + (value-high_bound);
    }
    if(value < low_bound){
        value = high_bound - (low_bound-value);
    }
    std::list<double*>::iterator it = modifiers.begin();
    for(; it!= modifiers.end(); it++){
        (*(*it)) = value;
    }
}

/*!
 * This function copies the value without linking it to the actual variable
 * @param inval The pointer ot the double that is updated
 */
void variable::copy(double* inval){
    *inval = value;
}

//! Deprecated, won't be around too long
void variable::parse(std::string inval){
    real8::parse(inval);
    inc_size=value;
    low_bound=0;
    high_bound=6.2382;
    value=0;
}
