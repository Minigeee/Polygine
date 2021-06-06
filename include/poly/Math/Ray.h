#ifndef POLY_RAY_H
#define POLY_RAY_H

#include <poly/Math/Vector3.h>

namespace poly
{


struct Ray
{
	Ray();

	Ray(const Vector3f& origin, const Vector3f& direction);

	Vector3f m_origin;
	Vector3f m_direction;
};


}

#endif