#pragma once
#ifndef EXAMPLE_TYPE_HPP
#define EXAMPLE_TYPE_HPP
#include <cmath>
#include <ostream>
#include "utils/type_operators.hpp"
//This class is an example of how to make a custom type fo use in the integrators
class example_type{
    double val1;
    double val2;
    public:
    double* vptr;
    example_type():vptr(&val1){};
    example_type(double v1, double v2): val1(v1), val2(v2), vptr(&val1){}
    //This class should break the rule of seperating into cpp and h files
    //even though it isn't a template. This allows the operator calls
    //to be inlined, and will provide a giant boost to performance
    inline example_type operator + (const example_type& inval) const{
        return example_type(val1+inval.val1, val2+inval.val2);
    }
    inline example_type operator - (const example_type& inval) const{
        return example_type(val1-inval.val1, val2-inval.val2);
    }
    inline example_type operator + (double inval) const{
        return example_type(val1+inval, val2+inval);
    }
    inline example_type operator - (double inval) const{
        return example_type(val1-inval, val2-inval);
    }
    inline example_type operator * (const example_type& inval) const{
        return example_type(val1*inval.val1, val2*inval.val2);
    }
    inline example_type operator / (const example_type& inval) const{
        return example_type(val1/inval.val1, val2/inval.val2);
    }
    inline example_type operator * (double inval) const{
        return example_type(val1*inval, val2*inval);
    }
    inline example_type operator / (double inval) const{
        return example_type(val1/inval, val2/inval);
    }
    friend struct get<example_type, 1>;
    friend struct get<example_type, 2>;
    friend double abs(const example_type& inval);
    friend std::ostream& operator <<(std::ostream& out, const example_type& dat);
};
inline double abs(const example_type& inval){
    return std::sqrt(inval.val1*inval.val1 + inval.val2*inval.val2);
}
//These allow for operators that go both ways (double * example_type) and (example_type * double)
inline example_type operator +(double val, const example_type & in){
    return in + val;
}
inline example_type operator *(double val, const example_type & in){
    return in * val;
}
inline example_type operator -(double val, const example_type & in){
    return (-1*in) + val;
}
//note the lack of a double/example_type operator-this might not always make since.
//
//If there is a constructor for examply_type that took a single double, hten this would implictly make sense
//However, this is not the case here and regardless it isn't needed
//

inline std::ostream& operator <<(std::ostream& out, const example_type& dat){
    out << "(" << dat.val1 << ", " << dat.val2 << ")";
    return out;
}

//This template specialization is used by the engine to determine what the
//underlying floating point type of the class should be
//in this case, it would be double since the class uses doubles
template<>
struct float_traits<example_type>{
    typedef double type;
    constexpr static size_t dim=2;
};
template<>
struct get<example_type, 1>{
    static inline double& pull(example_type& inval){
        return inval.val1;
    }
};
template<>
struct get<example_type, 2>{
    static inline double& pull(example_type& inval){
        return inval.val2;
    }
};
template<size_t dim>
struct get<example_type, dim>{
    //one of these will trip
    static_assert(dim > 2, "get created for example_type with a dimension greater than 2");
    static_assert(dim < 1, "get created for example_type with a dimension less than 1");
    static inline double pull(example_type& inval){
        return 0;
    }
};
#endif
