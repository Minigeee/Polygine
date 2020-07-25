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

    for (int i = 0; i < 100; ++i)
        LOG("Log #" + std::to_string(i));

    // Make sure all tasks are finished before quitting
    scheduler.finish();

    return 0;
}