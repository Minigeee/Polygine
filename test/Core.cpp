#include <poly/Core/Clock.h>
#include <poly/Core/HandleArray.h>
#include <poly/Core/ObjectPool.h>
#include <poly/Core/Profiler.h>
#include <poly/Core/Sleep.h>
#include <poly/Core/Time.h>
#include <poly/Core/TypeInfo.h>

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


TEST_CASE("Handle Array", "[HandleArray]")
{
    HandleArray<Uint32> arr(16);

    SECTION("Underlying memory")
    {
        REQUIRE(arr.capacity() == 16);
        REQUIRE(arr.size() == 0);
        REQUIRE(arr.isEmpty());
    }

    Handle h1 = arr.add(3);
    Handle h2 = arr.add(1);
    Handle h3 = arr.add(4);
    Handle h4 = arr.add(1);
    Handle h5 = arr.add(5);

    SECTION("Test accessor")
    {
        REQUIRE(arr[h1] == 3);
        REQUIRE(arr[h2] == 1);
        REQUIRE(arr[h3] == 4);
        REQUIRE(arr[h4] == 1);
        REQUIRE(arr[h5] == 5);
    }

    arr.remove(h2);
    arr.remove(h1);

    SECTION("Test remove")
    {
        REQUIRE(arr[h3] == 4);
        REQUIRE(arr[h4] == 1);
        REQUIRE(arr[h5] == 5);

        std::vector<Uint32>& data = arr.getData();
        REQUIRE(data.size() == 3);
        REQUIRE(data[0] == 1);
        REQUIRE(data[1] == 5);
        REQUIRE(data[2] == 4);
    }

    SECTION("Invalidate handles")
    {
        bool invalidated = false;
        try
        {
            Uint32 test = arr[h1];
        }
        catch (const char*)
        {
            invalidated = true;
        }
        REQUIRE(invalidated);

        invalidated = false;
        try
        {
            Uint32 test = arr[h2];
        }
        catch (const char*)
        {
            invalidated = true;
        }
        REQUIRE(invalidated);
    }
}

TEST_CASE("Time", "[Time]")
{
    Time t(0);

    SECTION("Conversions")
    {
        REQUIRE(Time::fromSeconds(1.0f).toMicroseconds() == 1000000);
        REQUIRE(Time::fromMilliseconds(1000).toMicroseconds() == 1000000);
    }

    SECTION("Operations")
    {
        Time a = Time::fromSeconds(1.2f);
        Time b = Time::fromSeconds(0.8f);
        const float epsilon = 0.00001f;

        REQUIRE(fabsf((a - b).toSeconds() - 0.4f) <= epsilon);
        REQUIRE(fabsf((a + b).toSeconds() - 2.0f) <= epsilon);
    }
}