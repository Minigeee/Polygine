#ifndef POLY_MATH_FUNCTIONS_H
#define POLY_MATH_FUNCTIONS_H

namespace poly
{


constexpr float PI = 3.14159265f;


///////////////////////////////////////////////////////////
/// \brief Convert degrees to radians
///
/// \param rad An angle in degrees
///
/// \return The same angle in radians
///
///////////////////////////////////////////////////////////
float rad(float deg);


///////////////////////////////////////////////////////////
/// \brief Convert radians to degrees
///
/// \param rad An angle in radians
///
/// \return The same angle in degrees
///
///////////////////////////////////////////////////////////
float deg(float rad);


///////////////////////////////////////////////////////////
/// \brief Clamp a value to a certain range
///
/// \param val The value to clamp to the range
/// \param min The minimum bound of the range
/// \param max The maximum bound of the range
///
/// \return The clamped value
///
///////////////////////////////////////////////////////////
template <typename T>
T clamp(T val, T min, T max);


///////////////////////////////////////////////////////////
/// \brief Linearly interpolate between two values
///
/// When interpolating, using a \a factor of 0.0 will return
/// the first parameter, using a \a factor of 1.0 will return
/// the second parameter, and a value in between will mix
/// the values.
///
/// \param a The first values to interpolate
/// \param b The second values to interpolate
/// \param factor The interpolation factor
///
/// \return The interpolated value
///
///////////////////////////////////////////////////////////
template <typename T>
T mix(T a, T b, float factor);


}

#include <poly/Math/Functions.inl>

#endif