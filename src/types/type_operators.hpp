/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
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
        //!helper function to reduce complexity of get function calls
        template<class U>
            static auto get(U& inval) -> decltype(get<U, dim>::pull(inval)){
                return ::get<U, dim>::pull(inval);
            }
    public:
        static_assert(dim >= 1, "A dimension less then one has been given to the apply structure");
        //no dimension checking here, that should be done by the getter function
        /*!
         * This function transforms a value, based upon the function func.
         * For each dimension, func is called with the current dimension of inval and
         * the dimension number, and returns the new value for the current dimension of inval.
         *
         * For example, if the data type was an array, the transform function would be equivilant to
         * @code
         * inval[0] = func(inval[0], 1);
         * inval[1] = func(inval[1], 2);
         * ...
         * inval[i] = func(inval[i], i+1);
         * @endcode
         * Note that in the above example, the loop is unrolled, and the dimensions are 1-indexed instead of zero indexed.
         * In addition, the compiler should inline the call to func is possible.
         *
         * In addition, note that the function isn't given knowledge of inval as a whole, simply each respective dimension.
         * This is because inval is modified during the process, so passing inval to the function wouldn't be meaningful.
         * 
         * You can get around this by binding a copy of inval to the passed function for use in calculations, like
         * @code
         * apply<T, dim>::transform(some_val, [some_val](T& inval, size_t dim){
         *      auto calc_val = do_calculation(some_val);
         *      inval.dimensions[dim-1] = calc_val * inval.dimensions[dim-1];
         * }
         * @endcode
         *
         * If the binding was be reference [&some_val], this would be incorrect as the value being used in the lambda
         * would be the same as the value being modified by transform
         *
         * @param inval The value being transformed
         * @param func The function object that operates on each dimension. Can be any callable object
         */
        template<class Lambda>
            static inline void transform(T& inval, Lambda&& func){
                std::forward<Lambda>(func)(get(inval), dim);
                apply<T, dim-1>::transform(inval, std::forward<Lambda>(func));
            }
        //!Version of transform where Lambda takes dim as a template parameter
        /*!
         * This version of transform takes a struct as a template parameter, where the
         * struct takes the dimension as a template parameter and has a function called apply
         * which takes the proper type as a reference parameter and does the coordinate transformation
         *
         * For example, one may have
         * @code
         * template<size_t dim>
         * struct do_calc{
         *      static void apply(double& inval){
         *          inval = calculation_based_on_dim(inval);
         *      }
         * };
         * ...
         * ...
         * ...
         * double x;
         * apply<double>::transform<do_calc>(x);
         * ...
         * ...
         *
         * @endcode
         */
        template<template<size_t> class Lambda>
            static inline void transform(T& inval){
                Lambda<dim>::apply(inval);
                apply<T, dim-1>::template transform<Lambda>(inval);
            }
        /*!
         * This function transforms an existing values, based upon the functions func and finish that have been passed
         * First, transform is called with inval and func, and then finish is applied
         *
         * @param finish the function applied after the calculations are finished
         */
        template<class Lambda, class Final>
            static inline void transform(T& inval, Lambda&& func, Final&& finish){
                apply<T, dim>::transform(inval, std::forward<Lambda>(func));
                std::forward<Final>(finish)(inval);
            }

        template<class Lambda, class Trans>
            static inline void map_into(const T& inval, Lambda&& func, Trans& outval){
                static_assert(float_traits<Trans>::dim == float_traits<T>::dim,
                        "The dimension of the Trans type is not equal to the dimension of the T type");
                get(outval) = std::forward<Lambda>(func)(get_cref<T, dim>::pull(inval), dim);
                apply<T, dim-1>::map_into(inval, std::forward<Lambda>(func), outval);
            }
        //!Generates a variable of type trans with the functor by mapping each coordinate of the first to the second
        template<class Lambda, class Trans>
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
    private:
    static constexpr size_t dim=1;
    template<class U>
        static auto get(U& inval) -> decltype(get<U, dim>::pull(inval)){
            return ::get<U, 1>::pull(inval);
        }
    public:
    template<class Lambda>
        static inline void transform(T& inval, Lambda&& func){
            std::forward<Lambda>(func)(get(inval), 1);
        }
    template<template<size_t> class Lambda>
        static inline void transform(T& inval){
            Lambda<1>::apply(inval);
        }
    template<class Lambda, class Final>
        static inline void transform(T& inval, Lambda&& func, Final&& finish){
            std::forward<Lambda>(func)(get(inval), 1);
            std::forward<Final>(finish)(inval);
        }
    template<class Lambda, class Trans>
        static inline void map_into(const T& inval, Lambda&& func, Trans& outval){
            get(outval) = std::forward<Lambda>(func)(get(inval), 1);
        }

};
#endif
