#include "defs.h"
#include "item.h"
#include "engineimp.h"
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
void variable::retrieve(void* inval, item* caller){
    double* d = (double*)inval;
    *d = value;
    //add pointer to the list of pointer for the given class
    modifiers[caller].insert(d);
}

/*!
 * This function increments the variable, and updates the relevant classes
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
    std::map<item*, std::set<double*> >::iterator mit, mremove;

    for(mit = modifiers.begin(); mit!= modifiers.end();){
        if(!(holder->item_exists(mit->first)) && mit->first){
            mremove = mit;
            mit++;
            modifiers.erase(mremove);
            continue;
        }
        std::set<double*>::iterator sit;
        for(sit=mit->second.begin(); sit!= mit->second.end(); sit++){
            *(*sit) = value;
        }
        if(mit->first){
            mit->first->update();
        }
        mit++;
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
