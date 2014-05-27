#pragma once
#ifndef EXAMPLE_TYPE_HPP
#define EXAMPLE_TYPE_HPP
#include <cmath>
#include <ostream>
//This class is an example of how to make a custom type fo use in the integrators
class example_type{
    double val1;
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
#endif
