#include "item_dim.h"
void item_dim::_do_mem_update(){
    err(this->name() + std::string(" of type ") + this->type() +
        std::string(" does not support a changing dimension."),
        "item_dim::_do_mem_update()", "parser/item.cpp", FATAL_ERROR);
}
/*
 * Postprocesses the item_dim class, which sets the dimension to the input variable dimension
 */
void item_dim::postprocess(std::map<std::string, std::shared_ptr<item> >& dat){
    int dimt;
    dat["dimension"]->retrieve(&dimt, this);
    if(dimt <= 0){
        std::string errmess = "dimension invalid, must be >= 0";
        err(errmess, "int_dim::postprocess", "item/item.cpp",
                dat["dimension"].get(), FATAL_ERROR);
    }
    dimension = (size_t)dimt;
}

/*!
 * Returns the dependencies of item_dim
 * \note
 * item_dim strongly depends on the input value integer dimension
 * @return Vector containing the dependencies of item_dim
 */
std::vector<std::string> item_dim::dependencies()const{
    std::vector<std::string> de;
    de.push_back("dimension");
    return de;
}
