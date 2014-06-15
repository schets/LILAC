#include "item.h"
#include "engineimp.h"
#include "item_heads.hpp"
/*!
 * This function prints the value of the current variable
 */
void variable::print() const{
    std::cout << "variable " << this->name() << "=" << value;
}

/*!
 * This function assigns the real value stored to the value pointed
 * at by inval. It also keeps track of the pointer and updates it whenever
 * the variable has changed. Passing a null pointer will result in the variable
 * not receiving updates
 */
void variable::_retrieve(retrieve_wrapper&& inval, item* caller){
    inval.check_and_get_type(typeid(double), &value);
    if(caller){
        double* d = (double*)inval.get_ptr();
        add_ref(*d, caller);
    }
}

void variable::add_ref(double& in, std::shared_ptr<item> inval){
    in=value;
    if(inval.use_count()){
        safe_mods[inval].insert(&in);
    }
    else{
#ifdef DEBUG
        err("Null pointer passed to variable add_ref", "variable::add_ref",
                "engine/variable.cpp", FATAL_ERROR);
#else
        err("Null pointer passed to variable add_ref", "variable::add_ref",
                "engine/variable.cpp", WARNING);
#endif
    }
}
void variable::add_ref(double& in, item* caller){
    if(caller){
        in = value;
        double* d = &in;
        //add pointer to the list of pointer for the given class
        if(holder->item_exists(caller->name())){
            safe_mods[std::weak_ptr<item>(holder->values[caller->name()].get_shared())].insert(d);
        }
        else{
            modifiers[caller].insert(d);
            err(std::string("Unsafe item ") + caller->name() + 
                    std::string(" being added to a variable modifier list"), 
                    "variable::retrieve", "engine/variable.cpp",
                    WARNING);
        }
    }
    else{
#ifdef DEBUG
        err("Null pointer passed to variable add_ref", "variable::add_ref",
                "engine/variable.cpp", FATAL_ERROR);
#else
        err("Null pointer passed to variable add_ref", "variable::add_ref",
                "engine/variable.cpp", WARNING);
#endif
    }
}
/*!
 * This function increments the variable, and updates the relevant classes
 */
void variable::set(double p){
    value = p;
    //deal with periodicity here
    while(value > high_bound){
        value = low_bound + (value-high_bound);
    }
    while(value < low_bound){
        value = high_bound - (low_bound-value);
    }
    //add items to engine's list of items that require updating
    //Removes items from list if they no longer exist in the engine
    {
        std::map<item*, std::set<double*> >::iterator mit, mremove;

        for(mit = modifiers.begin(); mit!= modifiers.end();){
            if(mit->first && !holder->item_exists(mit->first->name())){
                mremove = mit;
                ++mit;
                modifiers.erase(mremove);
                continue;
            }
            for(auto& sit:mit->second){
                (*sit) = value;
            }
            if(mit->first){
                holder->needs_updating(mit->first->name());
            }
            ++mit;
        }
    }
    {
        std::map<std::weak_ptr<item>, std::set<double*> >::iterator mit, mremove;

        for(mit = safe_mods.begin(); mit!= safe_mods.end();){
            if(!mit->first.use_count()){
                mremove = mit;
                mit++;
                safe_mods.erase(mremove);
                continue;
            }
            for(auto& sit:mit->second){
                (*sit) = value;
            }
            holder->needs_updating(mit->first.lock()->name());
            mit++;
        }
    }
}
void variable::inc(double p){
    set(value + p);
}
//! cover funtion for variable
void variable::inc(){
    inc(inc_size);
}

/*!
 * This function copies the value without linking it to the actual variable
 * @param inval The pointer to the double that is updated
 */
void variable::copy(double* inval){
    *inval = value;
}

void variable::parse(const std::string& inval){
    _double::parse(inval);
    inc_size=value;
    low_bound=0;
    high_bound=6.2382;
}

std::string variable::type()const{
    return "var";
}

