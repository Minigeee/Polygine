#include <poly/Core/Profiler.h>
#include <poly/Core/TypeInfo.h>

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Scene.h>

#include <iostream>
#include <math.h>

using namespace poly;


int main()
{
    Scene scene;

    Handle l1 = scene.addListener<int>(
        [](const int& event)
        {
            std::cout << "New Event: " << event << '\n';
        }
    );

    Handle l2 = scene.addListener<int>(
        [](const int& event)
        {
            std::cout << "I detect new event\n";
        }
    );

    scene.sendEvent(3);
    scene.sendEvent(1);
    scene.sendEvent(4);

    scene.removeListener<int>(l1);

    scene.sendEvent(3);
    scene.sendEvent(1);
    scene.sendEvent(4);

    return 0;
}

/*

TODO:
 - Threads class
 - Finish certain scheduler priorities

*/