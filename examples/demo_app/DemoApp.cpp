#include <poly/Core/TypeInfo.h>

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Scene.h>

#include <iostream>
#include <math.h>

using namespace poly;

int main()
{
    Scene scene;

    Entity a = scene.createEntity(2, 3.0f);
    Entity b = scene.createEntity(4.0f, 3);

    ComponentSet<int, float> set;
    set.set(3);
    set.set(3.14f);
    scene.createEntity(set);
    scene.createEntity<int, float>();

    int test = 0;

    return 0;
}