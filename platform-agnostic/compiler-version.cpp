#include <iostream>
#include <string>
#include <vector>

#ifdef _MSC_VER
#if _MSC_VER >= 1930
#define __VERSION__ "VS2022"
#elif _MSC_VER >= 1920
#define __VERSION__ "VS2019"
#elif _MSC_VER >= 1910
#define __VERSION__ "VS2017"
#elif _MSC_VER >= 1900
#define __VERSION__ "VS2015"
#elif _MSC_VER < 1900
#define __VERSION__ "Below VS2014"
#endif
#endif


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
    for (auto& el : vec)
    {
        os << el << ' ';
    }
    return os;
}

int main()
{
    std::vector<std::string> vec = {
        "Hello", "from", 
        #ifdef _MSC_VER
        "MSC", 
        #else
        "GCC/Clang", 
        #endif
        __VERSION__, "!" 
    };
    std::cout << vec << std::endl;
}