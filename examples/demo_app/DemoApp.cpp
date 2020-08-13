#include <poly/Core/Profiler.h>
#include <poly/Core/TypeInfo.h>

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Scene.h>
#include <poly/Engine/System.h>

#include <iostream>
#include <math.h>

using namespace poly;

class Test : public System<int, float>
{
public:
    Test(Scene* scene) :
        System<int, float>(scene)
    { }

    void process(float dt, const Entity::Id& id, int& i, float& f) override
    {
        ++i;
        f += 1.0f;
    }
};


int main()
{
    const int numTests = 1000;

    for (int i = 0; i < numTests; ++i)
    {
        std::vector<int> is(10002, 3);
        std::vector<float> fs(10002, 3.14f);

        START_PROFILING(base);

        for (int j = 0; j < 100; ++j)
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

    scene.createEntities(2000, 3, 3.14f);
    scene.createEntities(2000, 3, 3.14f, true);
    scene.createEntities(2000, 3, 3.14f, 45u);
    scene.createEntities(2000, 3, 3.14f, 4.0);
    scene.createEntities(2000, 3, 3.14f, &scene);

    for (int i = 0; i < numTests; ++i)
    {
        Test a(&scene);

        START_PROFILING(single);

        for (int j = 0; j < 100; ++j)
            a.update(0.0f);
    }

    const ProfilerData& a = Profiler::getData("main", "base");
    const ProfilerData& b = Profiler::getData("main", "single");

    std::cout << "Mean:   " << a.mean().toMicroseconds() << "us\nStd Dev: " << a.stdDev().toMicroseconds() << "us\n\n";
    std::cout << "Mean:   " << b.mean().toMicroseconds() << "us\nStd Dev: " << b.stdDev().toMicroseconds() << "us\n\n";

    return 0;
}