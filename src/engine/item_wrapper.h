#ifndef ITEM_WRAPPER_H
#define ITEM_WRAPPER_H
#include "item.h"
#include <type_traits>
class item;
//!This class wraps the actual item that has been input
/*
 * This class wraps the items that have been created by the system.
 * Items need wrapping to have proper dependency resolution. Internally,
 * all objects are stored in the same map. This means that local objects must have
 * names that differ from global objects. However, they still must be callable from
 */
class item_wrapper{
    std::shared_ptr<item> ptr;
    std::set<std::string> deps;
    public:
    template<class T, class = typename std::enable_if<std::is_base_of<item, T>::value>::type>
	    item_wrapper(const std::shared_ptr<T>& inv){
		    ptr = std::static_pointer_cast<item>(inv);
		    if(ptr.use_count()){
			    for(auto& str : inv->dependencies()){
				    deps.insert(str);
			    }
		    }
	    }
    item_wrapper(const std::shared_ptr<item>& inv);
    item_wrapper(const item_wrapper& inv);
    // item_wrapper(item_wrapper&& inv);
    item_wrapper();
    std::vector<std::string> dependencies() const;
    item* get();
    const item* get() const;
    item* operator ->();
    const item* operator ->() const;
    void add_dep(const std::string& indep);
    void replace_dep(const std::string& newdep, const std::string& olddep, bool must_rep=true);
    operator std::shared_ptr<item>();
    std::shared_ptr<item> get_shared();
    const std::shared_ptr<item> get_shared()const;

};
#endif