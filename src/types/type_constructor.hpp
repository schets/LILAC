#pragma once
#ifndef TYPE_CONSTRUCTOR_HPP
#define TYPE_CONSTRUCTOR_HPP
#include "vartype.hpp"
#include "defs.hpp"
#include "types/example_type.hpp"
#include <type_traits>
//
//There has to be a better way to combine this and satisfy the compiler...
//

/*
 * Function to iterate over a list of types, helps satisfy the compiler 
 */

//!class to designate a whitelist creator
class typelist_checker{};
//!class to designate a blacklist
class blacklist_checker{};
//!Class for iterating over a list of types and creating the corresponding template class
template<class ...P>
class typelist:public typelist_checker{
    template<class Tout, template<class> class Toh, template <class, class> class Tcr, class Tf, class ...Tl>
        static  Toh<Tout>* __create(const vartype* tval){

            static_assert(std::is_floating_point<typename float_traits<Tf>::type>::value,
                    "The float_traits of the current class is not a floating point type");
            if(tval->compare<Tf>()){
                return new Tcr<Tf, Tout>();
            }
            else return __create<Tout, Toh, Tcr, Tl...>(tval);
        }
    template<class Tout, template<class> class Toh, template <class, class> class Tcr>
        static  Toh<Tout>* __create(const vartype* tval){
            //has failed creation process
            std::string val("Type ");
            val.append(tval->vname());
            val.append(" is not recognized as a valid input type for ");
            val.append(typeid(Tout).name());
            err(val, "typelist::__create", "utils/type_constructor.hpp", FATAL_ERROR);
            return 0;//This simply satisfies static analyzers, err will always exit in this case
        }
    template<class Tout, template<class> class Toh, template<class, class> class Tcr>
        static void create(Toh<Tout>** in, const vartype* val){
            (*in)= __create<Tout, Toh, Tcr, P...>(val);
        }
    public:
    typelist(){};
    template<template<class> class U>
        friend class type_constructor;
};
//!This class provides a type blacklist that can be passed to the function
template<class ...P>
class blacklist:public blacklist_checker{
    //The extra template parameter seems to help the compiler
    // instantiate the recursion so it stays. It isn't part of the outer face so
    // whatever works.
    template<class Tout, class Tf, class ...Tl>
        static  bool __check(const vartype* tval){
            if(tval->compare<Tf>()){
                //has failed blacklist
                //
                return true;
            }
            else return __check<Tout, Tl...>(tval);
        }
    template<class Tout>
        static  bool __check(const vartype* tval){
            //has passed
            return false;//This simply satisfies static analyzers, err will always exit in this case
        }
    static bool check(const vartype* val){
        return __check<int, P...>(val);
    }
    public:
        blacklist(){};
    template< template<class> class U>
        friend class type_constructor;
};

//!Class for constructing statically-typed classes when type is determined at runtime
/*!
 *
 * This engine faces two issues-the numerical type (complex, double, float) is decided at runtime.
 * This problem is fairly simple to solve with runtime polymorphism-a base numerical class that
 * makes virtual calls out to add, multiply, etc functions
 *
 *
 * However, all of the functions need to be instantiated at compile time, to ensure optimal performance.
 * A solution based on runtime polymorphism would be unacceptably slow, probably by orders of magnitude
 *
 * To get past this, a type-independent class can hold a pointer to a template class,
 * where this class is selected at runtime. This hides the type-dependent functionality from the rest of the program.
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
 * in various places of code. Imagine creating a new type, and having to trawl the entire codebase editing this list
 *
 * However, we can create a type constructor.
 * This means that it takes a concrete type at runtime, such as complex, double, etc and returns a new concrete type
 * such as a rk45 integrator of type complex, or double. 
 * 
 * With templates, we can implement that in C++ as well, and as a bonus for arbitrary template and base classes
 * as long as they inherit from one another, and as long as they inherit from vartype.
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
 * \code
 * rk45* integ, *integ2;
 * rhs* some_typed_rhs; //in real code will have been instantiated
 * type_constructor<rk45_tmpl>::create(&integ, some_typed_rhs);
 * //or
 * integ2 = type_constructor<rk45_tmpl>::create<rk45>(some_typed_rhs);
 * \endcode  
 * 
 * You can also pass your own non-standard list of types to check, allowing for specialized
 * routines that don't need to operate on the standard numerical types
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
                virtual std::shared_ptr<Tout> create() = 0;
                virtual ~_ttype(){}
        };
    //!This is the actual creator class. It creates a pointer of Tin with the template parameter Tval
    template<class Tval, class Tout>
        class ttype: public _ttype<Tout>{
            public:
                //!Returns a Tin<Tval>* casted up to a Tout*
                std::shared_ptr<Tout> create(){
                    return std::make_shared<Tin<Tval> >();
                }
                virtual ~ttype(){}
        };

    public:

    /*!This defines the standard allowed types. Simply add your own type to the list if
     * it plays nicely with all the standard integrators. This means that it supports basic
     * arithmetic operations with itself and doubles, and has an abs function (absolute value)
     * definied in the global namespace
     */
    typedef typelist<comp, double, float, example_type> std_type_list;

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
     * Supported types are currently: comp, double, long double, and float. To add your own, simply add another
     * type to the typedef std_type_list. To prevent a type from being created, pass a blacklist
     *
     */
    template<class Tout, 
        class = typename std::enable_if<std::is_base_of<vartype, Tout>::value >::type>
            static void create(std::shared_ptr<Tout>* in, const vartype* tval){
                _ttype<Tout>* temp;
                std_type_list::create<Tout, _ttype, ttype>(&temp, tval);
                (*in) = temp->create();
                delete temp;
            }
    //This is an identical verison of above, except is accepts a blacklist against the standard typelist
    template<class Tl,class Tout, 
        class = typename std::enable_if<std::is_base_of<vartype, Tout>::value >::type,
        class = typename std::enable_if<std::is_base_of<blacklist_checker, Tl>::value >::type>
            static void create(std::shared_ptr<Tout>* in, const vartype* tval){
                _ttype<Tout>* temp;
                if(Tl::check(tval)){
                    std::string val("Type ");
                    val.append(tval->vname());
                    val.append(" is blacklisted as an input type for ");
                    val.append(typeid(Tout).name());
                    err(val, "type_constructor::create", "utils/type_constructor.hpp", FATAL_ERROR);
                }
                std_type_list::create<Tout, _ttype, ttype>(&temp, tval);
                (*in) = temp->create();
                delete temp;
            }

    //!Version of create that requires more template parameters, but directly returns a value
    //The list type is unspecified, so you can call this with a blacklist, a whitelist, or neither
    template<class Tout>
        static std::shared_ptr<Tout> create(const vartype* tval){
            std::shared_ptr<Tout> rval;
            type_constructor<Tin>::create<Tout>(&rval, tval);
            return rval;
        }
    //!shortened version of blacklist function
    template<class Tl, class Tout>
        static std::shared_ptr<Tout> create(const vartype* tval, const Tl blist){
            std::shared_ptr<Tout> rval;
            type_constructor<Tin>::create<Tl, Tout>(&rval, tval, blist);
            return rval;
        }
};
#endif
