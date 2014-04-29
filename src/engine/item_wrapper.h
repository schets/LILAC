#ifndef ITEM_WRAPPER_H
#define ITEM_WRAPPER_H
#include "item.h"
#include <type_traits>
class item;
class item_wrapper{
    std::shared_ptr<item> ptr;
    std::set<std::string> deps;
    public:
    template<class T, class = typename std::enable_if<std::is_base_of<item, T>::value>::type>
        item_wrapper(const std::shared_ptr<T>& inv):item_wrapper(std::static_pointer_cast<item>(inv)){
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
