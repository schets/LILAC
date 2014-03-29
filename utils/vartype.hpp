#pragma once
#ifndef VARTYPE_HPP
#define VARTYPE_HPP
#include <typeinfo>
//!Interface class to signal that an object deals with typed variables
/*!
 * This class forces the children classes to implement this method. Since most of the classes
 * in this program are abstract and untyped down until the very bottom of the tree, this effectively
 * forces the typed classes to implement the function. This functionality could also be directly implemented
 * in each of the children classes without dealing with an interface. However, this is clearer and
 * more self documenting. In addition, if one needs to group objects only by type, this provides a
 * powerful interface to do so.
 */
class vartype{
    public:
        virtual const std::type_info& vtype() = 0;
        virtual ~vartype(){};
};

#endif
