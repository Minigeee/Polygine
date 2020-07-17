#include <poly/Core/ObjectPool.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <iostream>
#include <vector>

using namespace poly;

///////////////////////////////////////////////////////////

TEST_CASE("Object Pool", "[ObjectPool]")
{
    ObjectPool pool;

    pool.setObjectSize(4);
    pool.setPageSize(128);

    SECTION("Setting parameters")
    {
        REQUIRE(pool.getObjectSize() == 4);
        REQUIRE(pool.getPageSize() == 128);
    }

    SECTION("Correct number of objects and pages")
    {
        REQUIRE(pool.getNumObjects() == 0);
        REQUIRE(pool.getNumPages() == 0);

        for (int i = 0; i < 128; ++i)
            pool.alloc();

        REQUIRE(pool.getNumObjects() == 128);
        REQUIRE(pool.getNumPages() == 1);

        pool.alloc();

        REQUIRE(pool.getNumObjects() == 129);
        REQUIRE(pool.getNumPages() == 2);

        for (int i = 0; i < 128; ++i)
            pool.alloc();

        REQUIRE(pool.getNumObjects() == 257);
        REQUIRE(pool.getNumPages() == 3);
    }

    SECTION("Resetting pool")
    {
        for (int i = 0; i < 128; ++i)
            pool.alloc();
        pool.reset();

        REQUIRE(pool.getNumObjects() == 0);
        REQUIRE(pool.getNumPages() == 0);

        for (int i = 0; i < 1024; ++i)
            pool.alloc();
        pool.reset();

        REQUIRE(pool.getNumObjects() == 0);
        REQUIRE(pool.getNumPages() == 0);
    }

    SECTION("Freeing objects")
    {
        std::vector<void*> ptrs;

        for (int i = 0; i < 128; ++i)
            ptrs.push_back(pool.alloc());

        for (int i = 0; i < 128; i += 2)
            pool.free(ptrs[i]);

        REQUIRE(pool.getNumObjects() == 64);
        REQUIRE(pool.getNumPages() == 1);

        for (int i = 0; i < 128; ++i)
            ptrs.push_back(pool.alloc());

        REQUIRE(pool.getNumObjects() == 192);
        REQUIRE(pool.getNumPages() == 2);
    }

    SECTION("Catch invalid settings")
    {
        pool.setObjectSize(0);
        REQUIRE(pool.alloc() == 0);

        pool.setObjectSize(3);
        REQUIRE(pool.alloc() == 0);

        pool.setObjectSize(5);
        void* ptr = pool.alloc();
        REQUIRE(ptr != 0);

        pool.free(0);
        REQUIRE(pool.getNumObjects() == 1);
        REQUIRE(pool.getNumPages() == 1);

        pool.free(ptr);
        REQUIRE(pool.getNumObjects() == 0);
        REQUIRE(pool.getNumPages() == 1);
    }

    SECTION("Test large number of pages")
    {
        pool.setPageSize(32);

        for (int i = 0; i < 32000; ++i)
            pool.alloc();

        REQUIRE(pool.getNumObjects() == 32000);
        REQUIRE(pool.getNumPages() == 1000);
    }

    SECTION("Test large pages")
    {
        pool.setPageSize(5000000);

        for (int i = 0; i < 5000000 * 2; ++i)
            pool.alloc();

        REQUIRE(pool.getNumObjects() == 5000000 * 2);
        REQUIRE(pool.getNumPages() == 2);
    }
}