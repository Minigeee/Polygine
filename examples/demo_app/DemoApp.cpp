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


int main()
{
    Logger::init("game.log");

    Scene scene;

    // ========================================================
    // ECS
    // ========================================================

    LOG("Hello World!");
    LOG_DEBUG("Hello World!");
    LOG_WARNING("Hello World!");
    LOG_ERROR("Hello World!");
    LOG_FATAL("Hello World!");

    // Create 100 entities with (int, float)
    LOG("Creating 100 entities");
    std::vector<Entity> entities = scene.createEntities<int, float>(100, 314, 3.14f);

    // Accessing individual components
    int* iPtr = entities[10].get<int>();                    // By using the Entity object
    iPtr = scene.getComponent<int>(entities[10].getId());   // or by using id

    // Apply changes using a system
    scene.system<int, float>(
        // This system will target entities that contain both integer and float
        [&](const Entity::Id& id, int& i, float& f)
        {
            ++i;
            f += 1.0f;

            // Queue an entity for removal
            scene.removeEntity(id);
        },

        // But exclude booleans
        ComponentTypeSet::create<bool>()
    );

    // Remove all queued entities
    scene.removeQueuedEntities();


    // ========================================================
    // Events
    // ========================================================

    TypeInfo::setTypeName<MsgEvent>("MsgEvent");

    // Add an event listener (std::function)
    Handle listener = scene.addListener<MsgEvent>(
        [](const MsgEvent& event)
        {
            LOG("MsgEvent: %s", event.m_message.c_str());
        }
    );

    // Send an event
    scene.sendEvent(MsgEvent{ "Hello World!" });

    // Remove the listener
    scene.removeListener<MsgEvent>(listener);



    return 0;
}