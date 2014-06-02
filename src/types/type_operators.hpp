#ifndef TYPE_OPERATORS_HPP
#define TYPE_OPERATORS_HPP
#include <complex>
#include <utility>
#include "float_traits.hpp"
template<int bound, int dim>
struct dim_check{
    constexpr static bool value = dim > 0 && dim <= bound;
};
//Getter structs to allow for generic and effecient retrieval of various parameters
//If only c++ allowed partial specialization of functions...
template<class T, int dim>
struct get{
    static_assert(dim > 0, "Dimension less than one passed to getter");
    static_assert(std::is_floating_point<T>::value, "Non floating-point type used with standard getter");
    static inline T& pull(T& inval){
        return inval;
    }
};
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
template<class T, size_t dim, bool is_first>
struct comp_get_helper{
    static inline typename float_traits<T>::type& pull(std::complex<T>& pullfrom){
            T& cur = ((T*)&pullfrom)[0];
            //is in the first value, use the current dim
            return get<T, dim>::pull(cur);
        }
};
template<class T, size_t dim>
struct comp_get_helper<T, dim, false>{
    static inline typename float_traits<T>::type& pull(std::complex<T>& pullfrom){
            T& cur = ((T*)&pullfrom)[1];
            //is in the second value, subtract dim of T to get proper index
            return get<T, dim-float_traits<T>::dim>::pull(cur);
        }
};
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
template<class T, int dim>
struct get_cref{
    typedef typename float_traits<T>::type real_type;
    const static inline real_type& pull(const T& inval){
        return get<T, dim>(const_cast<T&>(inval));
    }
};
template<class T, int dim>
struct get_val{
    typedef typename float_traits<T>::type real_type;
    static inline real_type pull(const T& inval){
        return get<T, dim>(const_cast<T&>(inval));
    }
};
//applies a function to each dimension along the way down
template<class T, size_t dim = float_traits<T>::dim>
struct apply{
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
    template<class Lambda, class Trans, class constr, class postop>
        static inline Trans map(const T& inval, Lambda&& func,
                postop&& finish=[](Trans&){}, constr&& build=[]{return Trans();}){
            Trans val = std::forward<constr>(build)();
            map_into(inval, std::forward<Lambda>(func), val);
            std::forward<postop>(finish)(val);
            return val;
        }
    //simple wrapper to make it simpler for pre-building classes and not doing post operations
    template<class Lambda, class Trans, class constr, class postop>
        static inline Trans map_build(const T& inval,  Lambda&& func, 
                constr&& build, postop&& finish=[](Trans&){}){
            return map(inval, std::forward<Lambda>(func),
                    std::forward<postop>(finish), std::forward<constr>(build));
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
