#include <cstddef>
#include <array>


template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
	return N;
}

int keyVales[] = {1,2,3,4,5,6,7}; // 7
int mappedVals[arraySize(keyVales)];
std::array <int, arraySize(keyVales)> mappedArray;


int main()
{
	return 0;
}