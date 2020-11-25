#ifndef POLY_PLANE_H
#define POLY_PLANE_H

#include <poly/Math/Vector3.h>

namespace poly
{

class Plane
{
public:
	Plane() = default;

	Plane(const Vector3f & n, float d);

	Plane(float a, float b, float c, float d);

	Vector3f reflectPoint(const Vector3f & p) const;

	Vector3f reflectVector(const Vector3f & v) const;

	Vector3f n;
	float d;
};

float dist(const Plane& plane, const Vector3f& p);

}

#endif