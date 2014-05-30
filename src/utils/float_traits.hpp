#ifndef FLOAT_TRAITS_HPP
#define FLOAT_TRAITS_HPP
#include <type_traits>
#include <complex>
template<class T, size_t n>
struct rep_type{
	constexpr static size_t size=n;
	T* vals;
	rep_type(T* iv):vals(iv){}
};
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
	static inline rep_type<type, 1> get_rep(type& inval){
		return rep_type<type, 1>(&inval);
	}
};
//catches numerical classes templatized by a floating point parameter
//like std::complex<T>
template<class T>
struct float_traits<std::complex<T>>{
	typedef typename float_traits<T>::type type;
	static inline rep_type<type, 2> get_rep(std::complex<T>& inval){
		return rep_type<type, 2>((type*)&inval);
	}
};
#endif
