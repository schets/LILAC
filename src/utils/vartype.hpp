#pragma once
#ifndef VARTYPE_HPP
#define VARTYPE_HPP
#include <typeinfo>
#include <string>
//!Interface class to signal that an object deals with typed variables
/*!
 * This class forces the children classes to implement this method. Since most of the classes
 * in this program are abstract and untyped down until the very bottom of the tree, this effectively
 * forces the typed classes to implement the function. This functionality could also be directly implemented
 * in each of the children classes without dealing with an interface. However, this is clearer and
 * more self documenting. In addition, if one needs to group objects only by type, this provides a
 * powerful interface to do so.
 *
 */
class vartype{
    public:
        virtual const std::type_info& vtype() const = 0;
        inline bool compare(const vartype* in) const{
            return vtype() == in->vtype();
        }
        inline bool compare(const vartype& in) const{
            return vtype() == in.vtype();
        }
        inline bool compare(const std::type_info& tinf){
            return vtype() == tinf;
        }
        template<typename T> inline bool compare() const{
            return vtype() == typeid(T);
        } 
        template<typename T> inline bool compare(const T& in) const{
            //could call typeid(T), but this plays more nicely with static analysis tools
            //and unused input parameters
            return vtype() == typeid(in);
        }
        inline std::string vname() const{
            return vtype().name();
        }
        /*
         * Functions that compare type internally used by the object
         * By default this is equal to the external type, but can be made different
         */
        virtual const std::type_info& vtype_internal() const{
            return vtype();
        }
        inline bool compare_internal(const vartype* in) const{
            return vtype_internal() == in->vtype_internal();
        }
        inline bool compare_internal(const vartype& in) const{
            return vtype_internal() == in.vtype_internal();
        }
        inline bool compare_internal(const std::type_info& tinf){
            return vtype_internal() == tinf;
        }
        template<typename T> inline bool compare_internal() const{
            return vtype_internal() == typeid(T);
        } 
        template<typename T> inline bool compare_internal(const T& in) const{
            //could call typeid(T), but this plays more nicely with static analysis tools
            //and unused input parameters
            return vtype_internal() == typeid(in);
        }
        inline std::string vname_internal() const{
            return vtype_internal().name();
        }

        virtual ~vartype(){};
};

#endif
