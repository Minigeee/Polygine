#include <poly/Math/Vector2.h>
#include <poly/Math/Vector3.h>

#include <iostream>
#include <math.h>

using namespace poly;

int main()
{
    Vector2f a(3, 4);
    Vector3f b(a, 5);

    Vector3f front(0, 0, -1);
    Vector3f up(0, 1, 0);
    Vector3f right(cross(front, up));

    return 0;
}