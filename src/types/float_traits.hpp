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
#ifndef FLOAT_TRAITS_HPP
#define FLOAT_TRAITS_HPP
#include <type_traits>
#include <complex>

//again, no specialization in class scopes bites me in the ass... save me c++17!!!!
//!Helper class for float_traits
template<class T, bool val>
struct sub_type{
    static_assert(val, "Unspecialized type used in float_traits");
    //This part helps a lot in preventing the rest of the code from exploding
    //in a mess of template error messages
    //since double seems to work with many things
    typedef double type;
};
//!Helper specialization for float_traits
template<class T>
struct sub_type<T, true>{
    typedef T type;
};
//!This class defines the basic floating_point type and dimension for a class
/*
 * This class provides a method for selecting a floating point-type to use given an arbitrary type.
 * float_traits<T>::type is the underlying floating point type, while dimension represents how big the type is.
 *
 * When passed a standard floating point type (double, etc) float_traits<T>::type is the same type, and dim is 1.
 *
 * An example on a different type would be std::complex<double>. float_traits<std::complex<double>>::type is double, and dim is 2.
 * See example_type for another example on a user defined type
 * \sa example_type
 */
template<class T>
struct float_traits{
    constexpr static bool cast_to_double=false;
    constexpr static bool is_fp = std::is_floating_point<T>::value;
    //allows for any floating point type to pass,
    //and catches unspecialized non-floating point types
    //Reults in cleaner error messages during instantiation of this class
    typedef typename sub_type<T, is_fp>::type type;
    constexpr static size_t dim=1;
};
template<>
struct float_traits<double>{
    constexpr static bool cast_to_double=true;
    constexpr static size_t dim = 1;
    typedef double type;
};
//!Specialization of float_traits for std::complex
template<class T>
struct float_traits<std::complex<T>>{
    constexpr static bool cast_to_double=float_traits<T>::cast_to_double;
    constexpr static size_t dim=2*float_traits<T>::dim;
    typedef typename float_traits<T>::type type;
};
#endif
