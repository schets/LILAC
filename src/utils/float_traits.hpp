#ifndef FLOAT_TRAITS_HPP
#define FLOAT_TRAITS_HPP
#include <type_traits>
//again, no specialization in class scopes bites me in the ass... save me c++17!!!!
template<class T, bool val>
class sub_type{
    public:
        static_assert(val, "Unspecialized type used in float_traits");
        //This part helps a lot in preventing the rest of the code from exploding
        //in a mess of template error messages
        //since double seems to work with many things
        typedef double type;
};
template<class T>
class sub_type<T, true>{
    public:
        typedef T type;
};
template<class T>
class float_traits{
    constexpr static bool is_fp = std::is_floating_point<T>::value;
    public:
    //allows for any floating point type to pass,
    //and catches unspecialized non-floating point types
    //Reults in cleaner error messages during instantiation of this class
    typedef typename sub_type<T, is_fp>::type type;
};
//catches numerical classes templatized by a floating point parameter
//like std::complex<T>
template< template<class> class temp_type, class T>
class float_traits<temp_type<T>>{
    public:
        typedef typename float_traits<T>::type type;
};
#endif
