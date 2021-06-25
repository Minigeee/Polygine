#ifndef POLY_MATH_FUNCTIONS_H
#define POLY_MATH_FUNCTIONS_H

namespace poly
{


constexpr float PI = 3.14159265f;

float rad(float deg);

float deg(float rad);

template <typename T>
T clamp(T val, T min, T max);


}

#include <poly/Math/Functions.inl>

#endif