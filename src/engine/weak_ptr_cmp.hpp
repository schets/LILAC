#ifndef WEAK_PTR_HPP
#define WEAK_PTR_HPP
#include <memory>
//NERSC's intel compiler can't compare weak ptrs, so I have to make a class to do it...
template<class T>
class weak_ptr_compare{
	std::weak_ptr<T> ptr;
	public:
	bool operator <(const weak_ptr_compare& inval){
		//this isn't concurrent so it should be safe

	}
};
#endif
