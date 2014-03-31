#pragma once
#ifndef TYPE_CONSTRUCTOR_HPP
#define TYPE_CONSTRUCTOR_HPP
#include "vartype.hpp"
#include "defs.hpp"
#include <type_traits>
//!Class for constructing statically-typed classes when type is determined at runtime
/*!
 *
 * This engine faces two issues-the numerical type (complex, double, float) is decided at runtime.
 * However, all of the functions need to be instantiated at compile time, to ensure optimal performance.
 * A type-independent class then holds a pointer to the polymorphic template class, 
 * hiding the type-dependent functionality from the rest of the program.
 *
 * There is still one problem, which is instantiating this template class. Currently,
 * this can just be with a series of if statements:
 * \code
 * if(type == complex){
 *     return new template_class<complex>
 * }
 * else if(type == double){
 *     return new template_class<double>
 * }
 * else if(type == float)...
 * \endcode
 * This works, but has two big problems. We must statically list the types that can be
 * transformed to in every location where this occurs. This is a headache, and prone to different lists
 * in various places of code
 *
 * However, taking a note from Haskell, we can create a type constructor, of kind (*->*).
 * This means that it takes a concrete type, such as complex, double, etc and returns a new concrete type
 * such as a rk45 integrator of type complex, or double. 
 * 
 * With templates, we can implement that in C++ as well, and as a bonus for arbitrary template and base classes
 * As long as they inherit from one another, and as long as they inherit from vartype.
 * The series of if statements is consolidated into one function
 *
 * For example, say you want to initiate an rk45 integrator to the same type as an rhs functions.
 * 
 * You have a base class rk45, which represents the family of rk45 integrators and is the polymorphic base class
 * 
 * You also have a template class rk45_tmpl<T>, which is a template class inheriting from rk45 and actually implementing
 * the integrator routines for type T.
 *
 * Your function also has access to the right hand side being integrated, but only knows about the type information at runtime.
 * Say that the type of ths rhs is T. To get a pointer to an rk45_tmpl<T> at runtime (in the form of an rk45*), you only have to
 * make a single function call.
 *
 * You can do either:
 *
 *
 * \param Tin the template class that is being instaniated
 *  
 */
template<template<class> class Tin>
class type_constructor{
    //! This is an internal polymorphic base class. It returns a pointer of type tout.
    template<class Tout>
    class _ttype{
        public:
            virtual Tout* create() = 0;
            virtual ~_ttype(){}
    };
    //!This is the actual creator class. It creates a pointer of Tin with the template parameter Tval
    template<class Tval, class Tout>
    class ttype: public _ttype<Tout>{
        public:
            //!Returns a Tin<Tval>* casted up to a Tout*
            Tout* create(){
                return new Tin<Tval>();
            }
            virtual ~ttype(){}
    };
    public:
        //!Creates a Tout* pointing to a Tin<type>*, where type is the type of the vartype tval.
        /*!
         *
         * This function is the core of the type constructor. If the type of the parameter tval is
         * type, then it returns a Tin<type>* casted up to a Tout. Notice that all instances of Tin
         * must be able to be casted up to a Tout. This is also only useful if the needed functionality
         * is polymorphic.
         *
         * This version of the function is nice because it doesn't require that the user specify a second
         * template parameter, namely the Tout parameter.
         *
         * Supported types are currently: comp, double, float. To add your own, simply add another
         * if statement to the list. Hopefully, this will soon be done by iterating over a typelist
         * instead of this list of if statements
         *
         *
         */
        template<class Tout, class = typename std::enable_if<std::is_base_of<vartype, Tout>::value >::type>
        static void create(Tout** in, const vartype* tval){
            _ttype<Tout>* temp;
            //Replace with typelist iteration
            if(tval->compare<comp>()){
                temp = new ttype<comp, Tout>();
            }
            else if(tval->compare<double>()){
                temp = new ttype<double, Tout>();
            }
            else if(tval->compare<float>()){
                temp = new ttype<float, Tout>();
            }
            else{
                std::string val("Type ");
                val.append(tval->vname());
                val.append(" is not recognized as a valid input type for ");
                val.append(typeid(Tout).name());
                err(val, "type_constructor::create", "utils/type_constructor.hpp", FATAL_ERROR);
                return;//This simply satisfies static analyzers, err will always exit in this case
            }
            (*in) = temp->create();
            delete temp;
        }
        //!Version of create that requires more template parameters, but directly returns a value
        template<class Tout> static Tout* create(const vartype* tval){
           Tout* rval;
           type_constructor<Tin>::create<Tout>(&rval, tval);
           return rval;
        }
};
#endif
