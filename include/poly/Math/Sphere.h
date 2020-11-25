#ifndef POLY_SPHERE_H
#define POLY_SPHERE_H

#include <poly/Math/Vector3.h>

namespace poly
{

class Sphere
{
public:
	Sphere();

	Sphere(const Vector3f& p, float r);

	Vector3f m_position;	//!< The position of the sphere
	float m_radius;			//!< The radius of the sphere
};

}

#endif