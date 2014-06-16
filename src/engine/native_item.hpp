#ifndef NATIVE_ITEM_HPP
#define NATIVE_ITEM_HPP
#include "item.h"
#include "utils/retrieve_checker.hpp"
#include "types/type_register.hpp"


//!This class provides a generic interface for making native types
/**
 * This class provides a method for creating a generic type.
 * The parameter T determines what the type is, and the string act_name
 * determines the type name
 */
template<class T, const char act_name[]>
class native_tmpl:public native_item{
    protected:
    T value;
    void _retrieve(retrieve_wrapper&& retval, item* caller){
        retval.check_and_get_type(typeid(T), &value);
    }
    public:
    std::string type() const{
        return act_name;
    }
    void parse(const std::string& inval){
        std::stringstream parser(inval);
        parser >> value;
    }

};

namespace __HIDER__{
    struct native_names{
        public:
            static constexpr char double_name[] = "double";
            static constexpr char float_name[] = "float";
            static constexpr char unsigned_name[] = "unsigned";
            static constexpr char string_name[] = "string";
            static constexpr char integer_name[] = "integer";
    };
}
typedef native_tmpl<double, __HIDER__::native_names::double_name> _double;
typedef native_tmpl<float, __HIDER__::native_names::float_name> _float;
typedef native_tmpl<std::string, __HIDER__::native_names::string_name> string;
typedef native_tmpl<size_t, __HIDER__::native_names::unsigned_name> _unsigned;
typedef native_tmpl<int, __HIDER__::native_names::integer_name> integer;
#endif
