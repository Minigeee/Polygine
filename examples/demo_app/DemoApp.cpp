#include <poly/Core/TypeInfo.h>

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Scene.h>

#include <iostream>
#include <math.h>

using namespace poly;

int main()
{
    Scene scene;

    Tuple<int, float> t;
    t.set(3);
    t.set(3.14f);

    std::cout << t.get<int>() << '\n';
    std::cout << get<1>(t) << '\n';

    return 0;
}