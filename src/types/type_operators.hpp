#ifndef TYPE_OPERATORS_HPP
#define TYPE_OPERATORS_HPP
#include <complex>
#include <utility>
#include "float_traits.hpp"
//I would recommend rolling your own static assert, so there is a more specific error message as well.
//But dim_check will always fail when value is false
//!value is true if dim>0 and dim <= bound. static_asserts on value as well.
template<int bound, int dim>
struct dim_check{
    constexpr static bool value = dim > 0 && dim <= bound;
    static_assert(value, "Dimension is outside of bounds");
};
//Getter structs to allow for generic and effecient retrieval of various parameters
//If only c++ allowed partial specialization of functions...
//!Generic get function. Fails to compile for non floating_point values and returns the reference for a floating point value
template<class T, int dim>
struct get{
    static_assert(dim > 0, "Dimension less than one passed to getter");
    static_assert(std::is_floating_point<T>::value, "Non floating-point type used with standard getter");
    static inline T& pull(T& inval){
        return inval;
    }
};
//!Getter specialization for compile-time sized arrays, does bounds checking
template<class T, int dim>
struct get<T[], dim>{
    static_assert(dim > 0, "Dimension less than one passed to getter");
    static_assert(std::is_floating_point<T>::value, "Non floating-point type used with standard getter");
    template<size_t N>
        static inline T& pull(T(& array)[N]){
            static_assert(dim <= N, "Dimension higher than array dimension used");
            return array[dim-1];
        }
};

//!Getter specialization for pointers, has no bounds checking except for <= 0
template<class T, int dim>
struct get<T*, dim>{
    static_assert(dim > 0, "Dimension less than one passed to getter");
    static_assert(std::is_floating_point<T>::value, "Non floating-point type used with standard getter");
    //adios bounds-checking...
    static inline T& pull(T* inval){
        return inval[dim-1];
    }
};

//These make it possible to extract from the complex values holding non-basic types 
//without violating dimension rules
//!Helper class for complex number getter
template<class T, size_t dim, bool is_first>
struct comp_get_helper{
    static inline typename float_traits<T>::type& pull(std::complex<T>& pullfrom){
            T& cur = ((T*)&pullfrom)[0];
            //is in the first value, use the current dim
            return get<T, dim>::pull(cur);
        }
};

//!Helper class for complex number getter
template<class T, size_t dim>
struct comp_get_helper<T, dim, false>{
    static inline typename float_traits<T>::type& pull(std::complex<T>& pullfrom){
            T& cur = ((T*)&pullfrom)[1];
            //is in the second value, subtract dim of T to get proper index
            return get<T, dim-float_traits<T>::dim>::pull(cur);
        }
};
//!Getter for complex numbers.
/*
 * This gets a value from a complex number. This is a somewhat complex proposition, since
 * the types can be nested (std::complex<example_type> for example).
 *
 * If you were to imagine the relevant floating_point values as an N dimensional array,=
 * The pull function would return the Nth value in that array (1-indexed, since this is dimensions)
 *
 * For example, an std::complex<example_type> would be [{val1, val2}, i*{val1, val2}];
 * get<2> would return val2 from the real part, while get<3> would return val1 from the imaginary part
 */
template<class T, int dim>
struct get<std::complex<T>, dim>{
    static_assert(dim_check<float_traits<std::complex<T>>::dim, dim>::value,
            "Dimension used outside the range of complex values");
    //having this function cleans up the code, and decreases the number of specializations
    static inline typename float_traits<T>::type& pull(std::complex<T>& inval){
        constexpr static bool is_first = dim <= float_traits<T>::dim;
        return comp_get_helper<T, dim, is_first>::pull(inval); 
    }
};

//!Returns a constant reference to the value
template<class T, int dim>
struct get_cref{
    typedef typename float_traits<T>::type real_type;
    const static inline real_type& pull(const T& inval){
        return get<T, dim>(const_cast<T&>(inval));
    }
};

//!Returns a copy of the value
template<class T, int dim>
struct get_val{
    typedef typename float_traits<T>::type real_type;
    static inline real_type pull(const T& inval){
        return get<T, dim>(const_cast<T&>(inval));
    }
};
//!Class containing functions for mapping over generic objects
template<class T, size_t dim = float_traits<T>::dim>
struct apply{
    private:
        //!helper functions to avoid lambda calls in default constructor, for gcc 4.7.2 compatibility
    public:
        static_assert(dim >= 1, "A dimension less then one has been given to the apply structure");
        //no dimension hecking here, that should be done by the getter function
        template<class Lambda>
            static inline void transform(T& inval, Lambda&& func){
                std::forward<Lambda>(func)(get<T, dim>::pull(inval), dim);
                apply<T, dim-1>::transform(inval, std::forward<Lambda>(func));
            }

        template<class Lambda, class Trans>
            static inline void map_into(const T& inval, Lambda&& func, Trans& outval){
                static_assert(float_traits<Trans>::dim == float_traits<T>::dim,
                        "The dimension of the Trans type is not euqal to the dimension of the T type");
                get<Trans, dim>::pull(outval) = std::forward<Lambda>(func)(get_cref<T, dim>::pull(inval), dim);
                apply<T, dim-1>::map_into(inval, std::forward<Lambda>(func), outval);
            }
        //!Generates a variable of type trans with the functor by mapping each coordinate of the first to the second
        template<class Lambda, class Trans, class constr, class postop>
            static inline Trans map(const T& inval, Lambda&& func){
                Trans val;
                map_into(inval, std::forward<Lambda>(func), val);
                return val;
            }
        //!Mapper with custom building and postoperation functors
        template<class Lambda, class Trans, class constr, class postop>
            static inline Trans map(const T& inval, Lambda&& func,
                    postop&& finish, constr&& build){
                Trans val = std::forward<constr>(build)();
                map_into(inval, std::forward<Lambda>(func), val);
                std::forward<postop>(finish)(val);
                return val;
            }
};
template<class T>
struct apply<T, 1>{
    template<class Lambda>
        static inline void transform(T& inval, Lambda&& func){
            std::forward<Lambda>(func)(get<T, 1>::pull(inval), 1);
        }
    template<class Lambda, class Trans>
        static inline void map_into(const T& inval, Lambda&& func, Trans& outval){
            get<Trans, 1>::pull(outval) = std::forward<Lambda>(func)(get<T, 1>::pull(inval), 1);
        }

};
#endif
