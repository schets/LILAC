#pragma once
#ifndef EXAMPLE_TYPE_HPP
#define EXAMPLE_TYPE_HPP
#include <cmath>
#include <ostream>
#include "type_operators.hpp"
//This class is an example of how to make a custom type fo use in the integrators
class example_type{
    double val1;
    //This prevents val1 and val2 from being next to each other in memory, meaning
    //we can't just overwrite this classe's memory with values to fill them
    //
    //This is an extremely contrived example, but it is possible that there may be something
    //that is different, and whose memory layout can't be changed (other code assumes a certain layout,
    //the function has internally calculated values, etc). Really, as long as there are values other than
    //the dimension values, you would have to write a custom getter anyways to prevent various operations
    //from stomping over the other parts of memory.
    double val1_and_val2_arent_next_to_each_other_in_memory_now;
    double val2;
    public:
    example_type(){};
    example_type(double v1, double v2): val1(v1), val2(v2){}
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
////!Specialization of float_traits for example_type
/*
 * This class defines the float_traits for example_type. Since the underlying values are
 * double, the type is double. Since the dimension is two, the dim variable is 2.
 */
template<>
struct float_traits<example_type>{
    typedef double type;
    constexpr static size_t dim=2;
};
//The next set of functions define the getter functions for example
//type. Since example type has 2 dimension, we must do two template specializations.
//The first one is for the first dimension, which is val1. The second returns val2,
//and any others return zero. However, the other dimensions will also cause errors to throw.
//Having a valid return type and function reduces compile errors.
//
//
//Why is this done at compile time, with template specialization? If we were to create a function that
//took a dimension at runtime and returned a value, that would require runtime decisions. However,
//the compile time decision allows the compiler to replace calls to get::pull
//directly with example_type.val1, etc. THis means no effeciency lost and no dependency on data layout.

//!Get function for first dimension
template<>
struct get<example_type, 1>{
    static inline double& pull(example_type& inval){
        return inval.val1;
    }
};

//!Get function for second dimension
template<>
struct get<example_type, 2>{
    static inline double& pull(example_type& inval){
        return inval.val2;
    }
};

//!Unspecialized get function that will fail static_assert
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
