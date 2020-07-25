#include <poly/Core/HandleArray.h>
#include <poly/Core/Logger.h>
#include <poly/Core/Profiler.h>
#include <poly/Core/Scheduler.h>
#include <poly/Core/Sleep.h>

#include <iostream>

using namespace poly;

int abc = 0;

void test()
{
    abc += 2;
}

void testSync()
{
    START_PROFILING(sync);
    for (int i = 0; i < 100; ++i)
        LOG("Hello World!");
    STOP_PROFILING(sync);
}

void testAsync()
{
    Scheduler scheduler;
    Logger::setScheduler(&scheduler);

    START_PROFILING(async);
    for (int i = 0; i < 100; ++i)
        LOG("Hello World!");
    STOP_PROFILING(async);

    scheduler.finish();
}

int main()
{
    Logger::init("game.log");

    for (int i = 0; i < 100; ++i)
        testSync();
    for (int i = 0; i < 100; ++i)
        testAsync();

    const ProfilerData& a = Profiler::getData("testSync", "sync");
    const ProfilerData& b = Profiler::getData("testAsync", "async");

    std::cout << "Mean:    " << a.mean().toMicroseconds() << "us\nStd Dev: " << a.stdDev().toMicroseconds() << "us\n\n";
    std::cout << "Mean:    " << b.mean().toMicroseconds() << "us\nStd Dev: " << b.stdDev().toMicroseconds() << "us\n\n";

    return 0;
}