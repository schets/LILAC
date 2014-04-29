#include "item_wrapper.h"
item* item_wrapper::operator->(){
    return ptr.get();
}
const item* item_wrapper::operator->()const{
    return ptr.get();
}
std::vector<std::string> item_wrapper::dependencies()const{
    return std::vector<std::string>(deps.begin(), deps.end());
}
void item_wrapper::add_dep(const std::string& indep){
    deps.insert(indep);
}
void item_wrapper::replace_dep(const std::string& newdep, const std::string& olddep, bool must_rep){
    if(deps.erase(olddep) || !must_rep){
        deps.insert(newdep);
    }
}
item* item_wrapper::get(){
    return ptr.get();
}
const item* item_wrapper::get()const{
    return ptr.get();
}
item_wrapper::operator std::shared_ptr<item>(){
    return ptr;
}
std::shared_ptr<item> item_wrapper::get_shared(){
    return ptr;
}
const std::shared_ptr<item> item_wrapper::get_shared()const{
    return ptr;
}
item_wrapper::item_wrapper(const std::shared_ptr<item>& inv){
    ptr = inv;
    if(ptr.use_count()){
        for(auto& str : inv->dependencies()){
            deps.insert(str);
        }
    }
}

item_wrapper::item_wrapper(const item_wrapper& inv):ptr(inv.ptr),
    deps(inv.deps){
    }
item_wrapper::item_wrapper(){
}
