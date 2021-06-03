#ifndef POLY_CAPSULE_COLLIDER_H
#define POLY_CAPSULE_COLLIDER_H

#include <poly/Physics/Collider.h>

namespace poly
{


class CapsuleCollider : public Collider
{
public:
	CapsuleCollider();

	CapsuleCollider(float radius, float height);

	void setRadius(float radius);

	void setHeight(float height);

	float getRadius() const;

	float getHeight() const;

private:
	float m_radius;
	float m_height;
};


}

#endif