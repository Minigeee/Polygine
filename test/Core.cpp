#include <poly/Core/ObjectPool.h>

#include <catch.hpp>

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";

    ObjectPool pool;
    pool.m_start = &pool;
    std::cout << pool.m_start << "\n";

    return 0;
}