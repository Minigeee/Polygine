#include <poly/Core/HandleArray.h>
#include <poly/Core/Logger.h>
#include <poly/Core/ObjectPool.h>
#include <poly/Math/Vector2.h>
#include <poly/Math/Vector3.h>

#include <iostream>
#include <math.h>

using namespace poly;

int main()
{
    Logger::init("game.log");

    LOG("Hello World!");

    ObjectPool pool(4);
    pool.alloc();
    pool.free(0);

    HandleArray<int> arr;
    Handle h = arr.add(1);
    arr.remove(h);

    int i = arr[h];

    return 0;
}