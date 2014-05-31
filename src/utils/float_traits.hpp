#ifndef FLOAT_TRAITS_HPP
#define FLOAT_TRAITS_HPP
#include <type_traits>
#include <complex>

//again, no specialization in class scopes bites me in the ass... save me c++17!!!!
template<class T, bool val>
struct sub_type{
    static_assert(val, "Unspecialized type used in float_traits");
    //This part helps a lot in preventing the rest of the code from exploding
    //in a mess of template error messages
    //since double seems to work with many things
    typedef double type;
};
template<class T>
struct sub_type<T, true>{
    typedef T type;
};
template<class T>
struct float_traits{
    constexpr static bool is_fp = std::is_floating_point<T>::value;
    //allows for any floating point type to pass,
    //and catches unspecialized non-floating point types
    //Reults in cleaner error messages during instantiation of this class
    typedef typename sub_type<T, is_fp>::type type;
    constexpr static size_t dim=1;
};
//catches numerical classes templatized by a floating point parameter
//like std::complex<T>
template<class T>
struct float_traits<std::complex<T>>{
    constexpr static size_t dim=2*float_traits<T>::dim;
    typedef typename float_traits<T>::type type;
};
#endif
