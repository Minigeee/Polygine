#include <poly/Core/Profiler.h>
#include <poly/Core/TypeInfo.h>

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Scene.h>

#include <iostream>
#include <math.h>

using namespace poly;


int main()
{
    const int numTests = 1000;

    for (int i = 0; i < numTests; ++i)
    {
        std::vector<int> is(50000, 3);
        std::vector<float> fs(50000, 3.14f);

        START_PROFILING(base);

        for (int j = 0; j < 20; ++j)
        {
            for (int n = 0; n < is.size(); ++n)
            {
                ++is[n];
                fs[n] += 1.0f;
            }
        }
    }

    Scene scene;

    Scheduler scheduler;

    Entity a = scene.createEntities(50000, 3, 3.14f)[0];

    for (int i = 0; i < numTests; ++i)
    {
        START_PROFILING(single);

        for (int j = 0; j < 20; ++j)
        {
            scene.system<int, float>(
                [](const Entity::Id& id, int& i, float& f)
                {
                    ++i;
                    f += 1.0f;
                },

                ComponentTypeSet::create<bool>()
            );
        }
    }

    std::cout << *a.get<int>() << '\n';

    const ProfilerData& d1 = Profiler::getData("main", "base");
    const ProfilerData& d2 = Profiler::getData("main", "single");

    std::cout << "Mean:   " << d1.mean().toMicroseconds() << "us\nStd Dev: " << d1.stdDev().toMicroseconds() << "us\n\n";
    std::cout << "Mean:   " << d2.mean().toMicroseconds() << "us\nStd Dev: " << d2.stdDev().toMicroseconds() << "us\n\n";

    return 0;
}

/*

TODO:
 - Threads class
 - Finish certain scheduler priorities

*/