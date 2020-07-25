#include <poly/Core/Clock.h>
#include <poly/Core/HandleArray.h>
#include <poly/Core/Logger.h>
#include <poly/Core/Profiler.h>
#include <poly/Core/Scheduler.h>
#include <poly/Core/Sleep.h>

using namespace poly;

int main()
{
    Scheduler scheduler;

    Logger::init("game.log");
    Logger::setScheduler(&scheduler);
    Logger::setThreadName("Main thread");

    LOG("Starting demo...");
    
    LOG("Creating handle arrays...");

    HandleArray<int> arr;

    for (int i = 0; i < 100; ++i)
        Handle h = arr.add(i);


    LOG("Sleeping for 5 seconds...");

    Clock clock;
    sleep(Time::fromSeconds(5.0f));
    Time elapsed = clock.getElapsedTime();

    LOG("Done sleeping!");
    LOG_DEBUG("Actually slept for %fs", elapsed.toSeconds());


    LOG_WARNING("Now I will force crash!");
    sleep(2.0f);
    LOG_ERROR("Something went wrong...");
    abort();

    // Make sure all tasks are finished before quitting
    scheduler.finish();

    return 0;
}