#include <poly/Core/Profiler.h>
#include <poly/Core/TypeInfo.h>

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Scene.h>

#include <iostream>
#include <math.h>

using namespace poly;

struct MsgEvent
{
    std::string m_message;
};

void listener1(const MsgEvent& event)
{
    std::cout << "Listener 1: " << event.m_message << '\n';
}


int main()
{
    Scene scene;

    // Add an event listener (std::function)
    Handle listener = scene.addListener<MsgEvent>(
        [](const MsgEvent& event)
        {
            std::cout << "New message: " << event.m_message << '\n';
        }
    );

    scene.addListener<MsgEvent>(listener1);

    // Send an event
    scene.sendEvent(MsgEvent{ "Hello World!" });

    // Remove the listener
    scene.removeListener<MsgEvent>(listener);

    std::cout << TypeInfo::get<Scene>().m_hash << '\n';

    return 0;
}

/*

TODO:
 - Threads class
 - Finish certain scheduler priorities

*/