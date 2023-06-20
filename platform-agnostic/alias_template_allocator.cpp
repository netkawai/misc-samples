#include <list>

template<class T>
struct Mallocator
{
    typedef T value_type;

	// minimum requires
    T* allocate(std::size_t n)
    {
    }
 
    void deallocate(T* p, std::size_t n) noexcept
    {
    }
};

// MyAllocList<T>
// using instead of typedef
template<typename T> 
using MyAllocList = std::list<T, Mallocator<T>>;


// value
class Widget
{
	
};



int main()
{
	// client
	MyAllocList<Widget> lw;
	return 0;
}