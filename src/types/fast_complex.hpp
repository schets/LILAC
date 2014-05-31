#ifndef FAST_COMPLEX_HPP
#define FAST_COMPLEX_HPP
//helper class to eliminate operations
template<class T>
struct imag_unit final{};
template<class T>
struct imag_val final{
    T val;
};
template<class T>
struct fast_comp final{
    T rval;
    T ival;
    public:
    inline fast_comp(){}
    inline fast_comp(const T& r):rval(r){}
    inline fast_comp(const T& r, const T& i):rval(r), ival(i){}
    inline fast_comp(const fast_comp<T>& a):rval(a.rval), ival(a.ival){}
    inline fast_comp<T> operator +(const fast_comp<T>& a){
        return fast_comp<T>(rval+a.rval, ival+a.ival);
    }
    inline fast_comp<T> operator -(const fast_comp<T>& a){
        return fast_comp<T>(rval-a.rval, ival-a.ival);
    }
    inline fast_comp operator *(const fast_comp<T>& a){
    }
};
#endif
