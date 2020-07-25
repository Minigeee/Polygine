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

    LOG("Starting demo...");
    
    LOG("Creating handle arrays...");


    HandleArray<int> arr;

    for (int i = 0; i < 100; ++i)
        Handle h = arr.add(i);

    LOG_WARNING("Warning... Warning...");

    Handle h(10);
    arr.remove(h);

    int v = arr[Handle(10)];
    LOG_DEBUG(std::to_string(v));

    // Make sure all tasks are finished before quitting
    scheduler.finish();

    return 0;
}