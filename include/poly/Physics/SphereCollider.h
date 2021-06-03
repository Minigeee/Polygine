#ifndef POLY_SPHERE_COLLIDER_H
#define POLY_SPHERE_COLLIDER_H

#include <poly/Physics/Collider.h>

namespace poly
{


class SphereCollider : public Collider
{
public:
	SphereCollider();

	SphereCollider(float radius);

	void setRadius(float radius);

	float getRadius() const;

private:
	float m_radius;
};


}

#endif