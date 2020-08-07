#include <poly/Core/TypeInfo.h>

#include <iostream>
#include <math.h>

using namespace poly;

template <typename... Args>
std::vector<Uint32> test()
{
    std::vector<Uint32> v = { TypeInfo::id<Args>()... };
    return v;
}

int main()
{
    auto v = test<int, Uint32, Int32>();
    int a = 0;

    return 0;
}